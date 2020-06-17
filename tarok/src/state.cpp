/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/state.h"

#include <algorithm>

#include "src/game.h"

namespace tarok {

std::ostream& operator<<(std::ostream& os, const GamePhase& game_phase) {
  os << "GamePhase::";
  switch (game_phase) {
    case GamePhase::kCardDealing:
      os << "CardDealing";
      break;
    case GamePhase::kBidding:
      os << "Bidding";
      break;
    case GamePhase::kKingCalling:
      os << "KingCalling:";
      break;
    case GamePhase::kTalonExchange:
      os << "TalonExchange";
      break;
    case GamePhase::kTricksPlaying:
      os << "TricksPlaying";
      break;
    case GamePhase::kFinished:
      os << "Finished";
  }
  return os;
}

// state definition
TarokState::TarokState(std::shared_ptr<const open_spiel::Game> game)
    : open_spiel::State(game),
      tarok_parent_game_(std::static_pointer_cast<const TarokGame>(game)) {}

open_spiel::Player TarokState::CurrentPlayer() const {
  switch (current_game_phase_) {
    case GamePhase::kCardDealing:
      return open_spiel::kChancePlayerId;
    case GamePhase::kFinished:
      return open_spiel::kTerminalPlayerId;
    default:
      return current_player_;
  }
}

std::vector<open_spiel::Action> TarokState::LegalActions() const {
  // all card actions are encoded as 0, 1, ..., 52, 53 and correspond to card
  // indices wrt. tarok_parent_game_->card_deck_, card actions are returned:
  //   - in the king calling phase
  //   - by LegalActionsInTalonExchange() after the talon set is selected (i.e.
  //     when discarding the cards)
  //   - by LegalActionsInTricksPlaying()
  switch (current_game_phase_) {
    case GamePhase::kCardDealing:
      // return a dummy action due to implicit stochasticity
      return {0};
    case GamePhase::kBidding:
      return LegalActionsInBidding();
    case GamePhase::kKingCalling:
      return {29, 37, 45, 53};
    case GamePhase::kTalonExchange:
      return LegalActionsInTalonExchange();
    case GamePhase::kTricksPlaying:
      return LegalActionsInTricksPlaying();
    case GamePhase::kFinished:
      return {};
  }
}

std::vector<open_spiel::Action> TarokState::LegalActionsInBidding() const {
  // actions 1 - 12 correspond to contracts in tarok_parent_game_->contracts_
  // respectively, action 0 means pass
  auto it = std::max_element(players_bids_.begin(), players_bids_.end());
  int max_bid = *it;
  int max_bid_player = it - players_bids_.begin();

  std::vector<open_spiel::Action> actions;
  if (current_player_ == 0 && players_bids_.at(current_player_) == -1 &&
      AllButCurrentPlayerPassedBidding()) {
    // no bidding has happened before so forehand can
    // bid any contract but can't pass
    actions.insert(actions.end(), {1, 2});
  } else if (!AllButCurrentPlayerPassedBidding()) {
    // other players still playing
    actions.push_back(0);
  }

  for (int action = 3; action <= 12; action++) {
    if (num_players_ == 3 && action >= 5 && action <= 7) continue;
    if (action < max_bid) continue;
    if ((action > max_bid) ||
        (action == max_bid && current_player_ <= max_bid_player)) {
      actions.push_back(action);
    }
  }
  return actions;
}

std::vector<open_spiel::Action> TarokState::LegalActionsInTalonExchange()
    const {
  if (talon_.size() == 6) {
    // choosing one of the talon card sets where actions are encoded as
    // 0, 1, 2, etc. from left to right, i.e. 0 is the leftmost talon set
    // as returned by TalonSets()
    std::vector<open_spiel::Action> actions(
        6 / selected_contract_info_->num_talon_exchanges);
    std::iota(actions.begin(), actions.end(), 0);
    return actions;
  }
  // discarding the cards
  std::vector<open_spiel::Action> actions;
  for (auto const& action : players_cards_.at(current_player_)) {
    if (ActionToCard(action).points != 5) actions.push_back(action);
  }
  return actions;
}

std::vector<open_spiel::Action> TarokState::LegalActionsInTricksPlaying()
    const {
  if (trick_cards_.empty()) {
    // trick opening, i.e. the current player is choosing
    // the first card for this trick
    if (selected_contract_info_->is_negative)
      return RemovePagatIfNeeded(players_cards_.at(current_player_));
    return players_cards_.at(current_player_);
  } else {
    // trick following
    return LegalActionsInTricksPlayingFollowing();
  }
}

std::vector<open_spiel::Action>
TarokState::LegalActionsInTricksPlayingFollowing() const {
  auto [can_follow_suit, cant_follow_suit_but_has_tarok] =
      CanFollowSuitOrCantButHasTarok();

  CardSuit take_suit;
  if (can_follow_suit) {
    take_suit = ActionToCard(trick_cards_.front()).suit;
  } else if (cant_follow_suit_but_has_tarok) {
    take_suit = CardSuit::kTaroks;
  } else {
    // can't follow suit and doesn't have taroks so any card can be played
    return players_cards_.at(current_player_);
  }

  if (selected_contract_info_->is_negative)
    return TakeSuitFromPlayerCardsInNegativeContracts(take_suit);
  else
    return TakeSuitFromPlayerCardsInPositiveContracts(take_suit);
}

std::tuple<bool, bool> TarokState::CanFollowSuitOrCantButHasTarok() const {
  const TarokCard& opening_card = ActionToCard(trick_cards_.front());
  bool has_taroks = false;
  for (auto const& action : players_cards_.at(current_player_)) {
    const TarokCard& current_card = ActionToCard(action);
    if (current_card.suit == opening_card.suit) {
      // note that the second return value is irrelevant in this case
      return {true, false};
    }
    if (current_card.suit == CardSuit::kTaroks) {
      has_taroks = true;
    }
  }
  return {false, has_taroks};
}

std::vector<open_spiel::Action>
TarokState::TakeSuitFromPlayerCardsInNegativeContracts(CardSuit suit) const {
  bool player_has_pagat =
      ActionInActions(0, players_cards_.at(current_player_));
  if (player_has_pagat && ActionInActions(20, trick_cards_) &&
      ActionInActions(21, trick_cards_)) {
    // the emperor trick, i.e. pagat has to be played as it is the only card
    // that will win the trick
    return {0};
  }

  std::optional<open_spiel::Action> action_to_beat =
      ActionToBeatInNegativeContracts(suit);
  std::vector<open_spiel::Action> actions;

  if (action_to_beat) {
    const TarokCard& card_to_beat = ActionToCard(*action_to_beat);
    auto const& player_cards = players_cards_.at(current_player_);
    // a higher card only has to be played when the player actually has a higher
    // card otherwise any card of the suit can be played
    bool has_higher_card = false;
    for (auto const& action : player_cards) {
      const TarokCard& current_card = ActionToCard(action);
      if (current_card.suit == suit && current_card.rank > card_to_beat.rank) {
        has_higher_card = true;
        break;
      }
    }
    // collect the actual cards
    for (auto const& action : player_cards) {
      const TarokCard& current_card = ActionToCard(action);
      if (current_card.suit == suit &&
          (!has_higher_card || current_card.rank > card_to_beat.rank)) {
        actions.push_back(action);
      }
    }
  } else {
    // no need to beat any card so simply return all cards of the correct suit
    actions = TakeSuitFromPlayerCardsInPositiveContracts(suit);
  }

  if (player_has_pagat)
    return RemovePagatIfNeeded(actions);
  else
    return actions;
}

std::optional<open_spiel::Action> TarokState::ActionToBeatInNegativeContracts(
    CardSuit suit) const {
  // there are two cases where no card has to be beaten; the player is following
  // a colour suit and there is already at least one tarok in trick_cards_ or
  // the player is forced to play a tarok and there are no taroks in
  // trick_cards_
  bool tarok_in_trick_cards = false;
  for (auto const& action : trick_cards_) {
    if (ActionToCard(action).suit == CardSuit::kTaroks) {
      tarok_in_trick_cards = true;
      break;
    }
  }
  if ((suit != CardSuit::kTaroks && tarok_in_trick_cards) ||
      (suit == CardSuit::kTaroks && !tarok_in_trick_cards)) {
    return {};
  }
  // the specified suit should be present in trick_cards_ from here on because
  // it is either a suit of the opening card or CardSuit::kTaroks with existing
  // taroks in trick_cards_
  open_spiel::Action action_to_beat = trick_cards_.front();
  for (int i = 1; i < trick_cards_.size(); i++) {
    const TarokCard& card_to_beat = ActionToCard(action_to_beat);
    const TarokCard& current_card = ActionToCard(trick_cards_.at(i));
    if (current_card.suit == suit && current_card.rank > card_to_beat.rank)
      action_to_beat = trick_cards_.at(i);
  }
  return action_to_beat;
}

std::vector<open_spiel::Action> TarokState::RemovePagatIfNeeded(
    const std::vector<open_spiel::Action>& actions) const {
  if (actions.size() > 1) {
    // mustn't play pagat unless it's the only card, note that actions
    // can be all player's cards or a subset already filtered by the caller
    std::vector<open_spiel::Action> actions_no_pagat;
    for (auto const& action : actions) {
      if (action != 0) actions_no_pagat.push_back(action);
    }
    return actions_no_pagat;
  }
  return actions;
}

std::vector<open_spiel::Action>
TarokState::TakeSuitFromPlayerCardsInPositiveContracts(CardSuit suit) const {
  std::vector<open_spiel::Action> actions;
  for (auto const& action : players_cards_.at(current_player_)) {
    if (ActionToCard(action).suit == suit) actions.push_back(action);
  }
  return actions;
}

std::string TarokState::ActionToString(open_spiel::Player player,
                                       open_spiel::Action action_id) const {
  switch (current_game_phase_) {
    case GamePhase::kCardDealing:
      // return a dummy action due to implicit stochasticity
      return "Deal";
    case GamePhase::kBidding:
      if (action_id == 0) return "Pass";
      return tarok_parent_game_->contracts_.at(action_id - 1).name;
    case GamePhase::kKingCalling:
    case GamePhase::kTricksPlaying:
      return CardActionToString(action_id);
    case GamePhase::kTalonExchange:
      if (talon_.size() == 6) return absl::StrCat("Talon set ", action_id + 1);
      return CardActionToString(action_id);
    case GamePhase::kFinished:
      return "";
  }
}

std::string TarokState::CardActionToString(open_spiel::Action action_id) const {
  return ActionToCard(action_id).ToString();
}

std::string TarokState::ToString() const {
  // todo: implement
  return "";
}

bool TarokState::IsTerminal() const {
  return current_game_phase_ == GamePhase::kFinished;
}

std::vector<double> TarokState::Returns() const {
  // todo: implement
  return {};
}

std::string TarokState::InformationStateString(
    open_spiel::Player player) const {
  // todo: implement
  return "";
}

std::unique_ptr<open_spiel::State> TarokState::Clone() const {
  // todo: implement
  return nullptr;
}

open_spiel::ActionsAndProbs TarokState::ChanceOutcomes() const {
  if (current_game_phase_ == GamePhase::kCardDealing) {
    // return a dummy action with probability 1 due to implicit stochasticity
    return {{0, 1.0}};
  }
  return {};
}

GamePhase TarokState::CurrentGamePhase() const { return current_game_phase_; }

std::vector<open_spiel::Action> TarokState::PlayerCards(
    open_spiel::Player player) const {
  if (current_game_phase_ == GamePhase::kCardDealing) return {};
  return players_cards_.at(player);
}

Contract TarokState::SelectedContract() const {
  if (current_game_phase_ == GamePhase::kCardDealing ||
      current_game_phase_ == GamePhase::kBidding) {
    return Contract::kNotSelected;
  }
  return selected_contract_info_->contract;
}

std::vector<open_spiel::Action> TarokState::Talon() const { return talon_; }

std::vector<std::vector<open_spiel::Action>> TarokState::TalonSets() const {
  if (current_game_phase_ != GamePhase::kTalonExchange) return {};

  int num_talon_sets =
      talon_.size() / selected_contract_info_->num_talon_exchanges;
  std::vector<std::vector<open_spiel::Action>> talon_sets;
  talon_sets.reserve(num_talon_sets);

  auto begin = talon_.begin();
  for (int i = 0; i < num_talon_sets; i++) {
    talon_sets.push_back(std::vector<open_spiel::Action>(
        begin, begin + selected_contract_info_->num_talon_exchanges));
    std::advance(begin, selected_contract_info_->num_talon_exchanges);
  }
  return talon_sets;
}

std::vector<open_spiel::Action> TarokState::TrickCards() const {
  return trick_cards_;
}

void TarokState::DoApplyAction(open_spiel::Action action_id) {
  if (!ActionInActions(action_id, LegalActions())) {
    open_spiel::SpielFatalError(absl::StrCat(
        "Action ", action_id, " is not valid in the current state."));
  }
  switch (current_game_phase_) {
    case GamePhase::kCardDealing:
      DoApplyActionInCardDealing();
      break;
    case GamePhase::kBidding:
      DoApplyActionInBidding(action_id);
      break;
    case GamePhase::kKingCalling:
      DoApplyActionInKingCalling(action_id);
      break;
    case GamePhase::kTalonExchange:
      DoApplyActionInTalonExchange(action_id);
      break;
    case GamePhase::kTricksPlaying:
      DoApplyActionInTricksPlaying(action_id);
      break;
    case GamePhase::kFinished:
      open_spiel::SpielFatalError("Calling DoApplyAction in a terminal state.");
  }
}

void TarokState::DoApplyActionInCardDealing() {
  // do the actual sampling here due to implicit stochasticity
  // todo: deal again if any player without taroks
  std::tie(talon_, players_cards_) =
      DealCards(num_players_, tarok_parent_game_->RNG());

  current_game_phase_ = GamePhase::kBidding;
  players_bids_.reserve(num_players_);
  players_bids_.insert(players_bids_.end(), num_players_, -1);

  // lower player indices correspond to higher bidding priority,
  // i.e. 0 is the forehand, num_players - 1 is the dealer
  current_player_ = 1;
}

void TarokState::DoApplyActionInBidding(open_spiel::Action action_id) {
  players_bids_.at(current_player_) = action_id;
  if (AllButCurrentPlayerPassedBidding()) {
    FinishBiddingPhase(action_id);
  } else {
    do {
      NextPlayer();
    } while (players_bids_.at(current_player_) == 0);
  }
}

bool TarokState::AllButCurrentPlayerPassedBidding() const {
  for (int i = 0; i < num_players_; i++) {
    if (i == current_player_) continue;
    if (players_bids_.at(i) != 0) return false;
  }
  return true;
}

void TarokState::FinishBiddingPhase(open_spiel::Action action_id) {
  declarer_ = current_player_;
  selected_contract_info_ = &tarok_parent_game_->contracts_.at(action_id - 1);

  if (num_players_ == 4 && selected_contract_info_->needs_king_calling)
    current_game_phase_ = GamePhase::kKingCalling;
  else if (selected_contract_info_->NeedsTalonExchange())
    current_game_phase_ = GamePhase::kTalonExchange;
  else
    StartTricksPlayingPhase();

  players_collected_cards_.reserve(num_players_);
  for (int i = 0; i < num_players_; i++) {
    players_collected_cards_.push_back(std::vector<open_spiel::Action>());
  }
}

void TarokState::DoApplyActionInKingCalling(open_spiel::Action action_id) {
  for (int i = 0; i < num_players_; i++) {
    if (i == current_player_) {
      continue;
    } else if (ActionInActions(action_id, players_cards_.at(i))) {
      declarer_partner_ = i;
      break;
    }
  }
  current_game_phase_ = GamePhase::kTalonExchange;
}

void TarokState::DoApplyActionInTalonExchange(open_spiel::Action action_id) {
  auto& player_cards = players_cards_.at(current_player_);

  if (talon_.size() == 6) {
    // choosing one of the talon card sets
    int num_talon_exchanges = selected_contract_info_->num_talon_exchanges;
    int pos = action_id * num_talon_exchanges;
    for (int i = pos; i < pos + num_talon_exchanges; i++) {
      player_cards.push_back(talon_.at(i));
    }
    std::sort(player_cards.begin(), player_cards.end());
    talon_.erase(talon_.begin() + pos,
                 talon_.begin() + pos + num_talon_exchanges);
  } else {
    // discarding the cards
    MoveActionFromTo(action_id, &player_cards,
                     &players_collected_cards_.at(current_player_));
    if (player_cards.size() == 48 / num_players_) {
      StartTricksPlayingPhase();
    }
  }
}

void TarokState::StartTricksPlayingPhase() {
  current_game_phase_ = GamePhase::kTricksPlaying;
  if (selected_contract_info_->declarer_starts)
    current_player_ = declarer_;
  else
    current_player_ = 0;
}

void TarokState::DoApplyActionInTricksPlaying(open_spiel::Action action_id) {
  MoveActionFromTo(action_id, &players_cards_.at(current_player_),
                   &trick_cards_);
  if (trick_cards_.size() == num_players_) {
    ResolveTrick();
    if (players_cards_.at(current_player_).empty()) {
      current_game_phase_ = GamePhase::kFinished;
    }
  } else {
    NextPlayer();
  }
}

void TarokState::ResolveTrick() {
  open_spiel::Player trick_winner = ResolveTrickWinner();
  auto& trick_winner_collected_cards =
      players_collected_cards_.at(trick_winner);

  for (auto const& action : trick_cards_) {
    trick_winner_collected_cards.push_back(action);
  }
  if (selected_contract_info_->contract == Contract::kKlop &&
      talon_.size() > 0) {
    // add the "gift" talon card
    trick_winner_collected_cards.push_back(talon_.front());
    talon_.erase(talon_.begin());
  }
  trick_cards_.clear();
  current_player_ = trick_winner;
}

open_spiel::Player TarokState::ResolveTrickWinner() const {
  // compute the winning action index within trick_cards_
  int winning_action_i;
  if ((ActionInActions(0, trick_cards_) && ActionInActions(20, trick_cards_) &&
       ActionInActions(21, trick_cards_)) &&
      (selected_contract_info_->contract != Contract::kColourValatWithout ||
       ActionToCard(trick_cards_.front()).suit == CardSuit::kTaroks)) {
    // the emperor trick, i.e. pagat wins over mond and skis in all cases but
    // not in Contract::kColourValatWithout when a non-trump is led
    winning_action_i = std::find(trick_cards_.begin(), trick_cards_.end(), 0) -
                       trick_cards_.begin();
  } else {
    winning_action_i = 0;
    for (int i = 1; i < trick_cards_.size(); i++) {
      const TarokCard& winning_card =
          ActionToCard(trick_cards_.at(winning_action_i));
      const TarokCard& current_card = ActionToCard(trick_cards_.at(i));

      if (((current_card.suit == CardSuit::kTaroks &&
            selected_contract_info_->contract !=
                Contract::kColourValatWithout) ||
           current_card.suit == winning_card.suit) &&
          current_card.rank > winning_card.rank) {
        winning_action_i = i;
      }
    }
  }
  return TrickCardsIndexToPlayer(winning_action_i);
}

open_spiel::Player TarokState::TrickCardsIndexToPlayer(int index) const {
  open_spiel::Player player = current_player_;
  for (int i = 0; i < trick_cards_.size() - 1 - index; i++) {
    player -= 1;
    if (player == -1) player = num_players_ - 1;
  }
  return player;
}

void TarokState::NextPlayer() {
  current_player_ += 1;
  if (current_player_ == num_players_) current_player_ = 0;
}

bool TarokState::ActionInActions(
    open_spiel::Action action_id,
    const std::vector<open_spiel::Action>& actions) {
  return std::find(actions.begin(), actions.end(), action_id) != actions.end();
}

void TarokState::MoveActionFromTo(open_spiel::Action action_id,
                                  std::vector<open_spiel::Action>* from,
                                  std::vector<open_spiel::Action>* to) {
  from->erase(std::remove(from->begin(), from->end(), action_id), from->end());
  to->push_back(action_id);
}

const TarokCard& TarokState::ActionToCard(open_spiel::Action action_id) const {
  return tarok_parent_game_->card_deck_.at(action_id);
}

}  // namespace tarok
