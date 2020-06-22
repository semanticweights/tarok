/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/state.h"

#include <algorithm>
#include <cmath>

#include "src/game.h"

namespace tarok {

// state definition
TarokState::TarokState(std::shared_ptr<const open_spiel::Game> game)
    : open_spiel::State(game),
      tarok_parent_game_(std::static_pointer_cast<const TarokGame>(game)) {
  players_bids_.reserve(num_players_);
  players_bids_.insert(players_bids_.end(), num_players_, kInvalidBidAction);
  players_collected_cards_.reserve(num_players_);
  players_collected_cards_.insert(players_collected_cards_.end(), num_players_,
                                  std::vector<open_spiel::Action>());
}

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

bool TarokState::IsTerminal() const {
  return current_game_phase_ == GamePhase::kFinished;
}

GamePhase TarokState::CurrentGamePhase() const { return current_game_phase_; }

std::vector<open_spiel::Action> TarokState::PlayerCards(
    open_spiel::Player player) const {
  if (current_game_phase_ == GamePhase::kCardDealing) return {};
  return players_cards_.at(player);
}

ContractName TarokState::SelectedContractName() const {
  if (current_game_phase_ == GamePhase::kCardDealing ||
      current_game_phase_ == GamePhase::kBidding) {
    return ContractName::kNotSelected;
  }
  return selected_contract_->name;
}

std::vector<open_spiel::Action> TarokState::Talon() const { return talon_; }

std::vector<std::vector<open_spiel::Action>> TarokState::TalonSets() const {
  if (current_game_phase_ != GamePhase::kTalonExchange) return {};

  int num_talon_sets = talon_.size() / selected_contract_->num_talon_exchanges;
  std::vector<std::vector<open_spiel::Action>> talon_sets;
  talon_sets.reserve(num_talon_sets);

  auto begin = talon_.begin();
  for (int i = 0; i < num_talon_sets; i++) {
    talon_sets.push_back(std::vector<open_spiel::Action>(
        begin, begin + selected_contract_->num_talon_exchanges));
    std::advance(begin, selected_contract_->num_talon_exchanges);
  }
  return talon_sets;
}

std::vector<open_spiel::Action> TarokState::TrickCards() const {
  return trick_cards_;
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
      return {kKingOfHeartsAction, kKingOfDiamondsAction, kKingOfSpadesAction,
              kKingOfClubsAction};
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
  if (current_player_ == 0 &&
      players_bids_.at(current_player_) == kInvalidBidAction &&
      AllButCurrentPlayerPassedBidding()) {
    // no bidding has happened before so forehand can
    // bid any contract but can't pass
    actions.insert(actions.end(), {kBidKlopAction, kBidThreeAction});
  } else if (!AllButCurrentPlayerPassedBidding()) {
    // other players still playing
    actions.push_back(kBidPassAction);
  }

  for (int action = 3; action <= 12; action++) {
    if (num_players_ == 3 && action >= kBidSoloThreeAction &&
        action <= kBidSoloOneAction) {
      // skip solo contracts for three players
      continue;
    }
    if (action < max_bid) {
      continue;
    }
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
        6 / selected_contract_->num_talon_exchanges);
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
    if (selected_contract_->is_negative)
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

  if (selected_contract_->is_negative)
    return TakeSuitFromPlayerCardsInNegativeContracts(take_suit);
  else
    return TakeSuitFromPlayerCardsInPositiveContracts(take_suit);
}

std::tuple<bool, bool> TarokState::CanFollowSuitOrCantButHasTarok() const {
  const Card& opening_card = ActionToCard(trick_cards_.front());
  bool has_taroks = false;
  for (auto const& action : players_cards_.at(current_player_)) {
    const Card& current_card = ActionToCard(action);
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
      ActionInActions(kPagatAction, players_cards_.at(current_player_));
  if (player_has_pagat && ActionInActions(kMondAction, trick_cards_) &&
      ActionInActions(kSkisAction, trick_cards_)) {
    // the emperor trick, i.e. pagat has to be played as it is the only card
    // that will win the trick
    return {kPagatAction};
  }

  std::optional<open_spiel::Action> action_to_beat =
      ActionToBeatInNegativeContracts(suit);
  std::vector<open_spiel::Action> actions;

  if (action_to_beat) {
    const Card& card_to_beat = ActionToCard(*action_to_beat);
    auto const& player_cards = players_cards_.at(current_player_);
    // a higher card only has to be played when the player actually has a higher
    // card otherwise any card of the suit can be played
    bool has_higher_card = false;
    for (auto const& action : player_cards) {
      const Card& current_card = ActionToCard(action);
      if (current_card.suit == suit && current_card.rank > card_to_beat.rank) {
        has_higher_card = true;
        break;
      }
    }
    // collect the actual cards
    for (auto const& action : player_cards) {
      const Card& current_card = ActionToCard(action);
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
    const Card& card_to_beat = ActionToCard(action_to_beat);
    const Card& current_card = ActionToCard(trick_cards_.at(i));
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
      if (action != kPagatAction) actions_no_pagat.push_back(action);
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
      return ContractNameToString(
          tarok_parent_game_->contracts_.at(action_id - 1).name);
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

open_spiel::ActionsAndProbs TarokState::ChanceOutcomes() const {
  if (current_game_phase_ == GamePhase::kCardDealing) {
    // return a dummy action with probability 1 due to implicit stochasticity
    return {{0, 1.0}};
  }
  return {};
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
    } while (players_bids_.at(current_player_) == kBidPassAction);
  }
}

bool TarokState::AllButCurrentPlayerPassedBidding() const {
  for (int i = 0; i < num_players_; i++) {
    if (i == current_player_) continue;
    if (players_bids_.at(i) != kBidPassAction) return false;
  }
  return true;
}

void TarokState::FinishBiddingPhase(open_spiel::Action action_id) {
  declarer_ = current_player_;
  selected_contract_ = &tarok_parent_game_->contracts_.at(action_id - 1);

  if (num_players_ == 4 && selected_contract_->needs_king_calling)
    current_game_phase_ = GamePhase::kKingCalling;
  else if (selected_contract_->NeedsTalonExchange())
    current_game_phase_ = GamePhase::kTalonExchange;
  else
    StartTricksPlayingPhase();
}

void TarokState::DoApplyActionInKingCalling(open_spiel::Action action_id) {
  called_king_ = action_id;
  if (ActionInActions(action_id, talon_)) {
    called_king_in_talon_ = true;
  } else {
    for (int i = 0; i < num_players_; i++) {
      if (i == current_player_) {
        continue;
      } else if (ActionInActions(action_id, players_cards_.at(i))) {
        declarer_partner_ = i;
        break;
      }
    }
  }
  current_game_phase_ = GamePhase::kTalonExchange;
}

void TarokState::DoApplyActionInTalonExchange(open_spiel::Action action_id) {
  auto& player_cards = players_cards_.at(current_player_);

  if (talon_.size() == 6) {
    // choosing one of the talon card sets
    int set_begin = action_id * selected_contract_->num_talon_exchanges;
    int set_end = set_begin + selected_contract_->num_talon_exchanges;

    bool mond_in_talon = ActionInActions(kMondAction, talon_);
    bool mond_in_selected_talon_set = false;
    for (int i = set_begin; i < set_end; i++) {
      player_cards.push_back(talon_.at(i));
      if (talon_.at(i) == kMondAction) mond_in_selected_talon_set = true;
    }
    if (mond_in_talon && !mond_in_selected_talon_set) {
      // the captured mond penalty applies if mond is in talon and not part of
      // the selected set
      captured_mond_player_ = current_player_;
    }

    std::sort(player_cards.begin(), player_cards.end());
    talon_.erase(talon_.begin() + set_begin, talon_.begin() + set_end);
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
  if (selected_contract_->declarer_starts)
    current_player_ = declarer_;
  else
    current_player_ = 0;
}

void TarokState::DoApplyActionInTricksPlaying(open_spiel::Action action_id) {
  MoveActionFromTo(action_id, &players_cards_.at(current_player_),
                   &trick_cards_);
  if (trick_cards_.size() == num_players_) {
    ResolveTrick();
    if (players_cards_.at(current_player_).empty() ||
        ((selected_contract_->name == ContractName::kBeggar ||
          selected_contract_->name == ContractName::kOpenBeggar) &&
         current_player_ == declarer_) ||
        ((selected_contract_->name == ContractName::kColourValatWithout ||
          selected_contract_->name == ContractName::kValatWithout) &&
         current_player_ != declarer_)) {
      current_game_phase_ = GamePhase::kFinished;
    }
  } else {
    NextPlayer();
  }
}

void TarokState::ResolveTrick() {
  auto [trick_winner, winning_action] = ResolveTrickWinnerAndWinningAction();
  auto& trick_winner_collected_cards =
      players_collected_cards_.at(trick_winner);

  for (auto const& action : trick_cards_) {
    trick_winner_collected_cards.push_back(action);
  }

  if (selected_contract_->name == ContractName::kKlop && talon_.size() > 0) {
    // add the "gift" talon card in klop
    trick_winner_collected_cards.push_back(talon_.front());
    talon_.erase(talon_.begin());
  } else if (winning_action == called_king_ && called_king_in_talon_) {
    // declearer won the trick with the called king that was in talon so all
    // of the talon cards belong to the declearer (note that this is only
    // possible when talon exchange actually happened in the past)
    bool mond_in_talon = false;
    for (auto const& action : talon_) {
      trick_winner_collected_cards.push_back(action);
      if (action == kMondAction) mond_in_talon = true;
    }
    if (mond_in_talon) {
      // the called king and mond were in different parts of the talon and
      // declearer selected the set with the king plus won the mond as
      // part of the obtained talon remainder, negating the captured mond
      // penalty obtained during DoApplyActionInTalonExchange()
      captured_mond_player_ = open_spiel::kInvalidPlayer;
    }
    talon_.clear();
  } else if ((selected_contract_->NeedsTalonExchange() ||
              selected_contract_->name == ContractName::kSoloWithout) &&
             (winning_action == kSkisAction ||
              winning_action == kPagatAction)) {
    // check if mond is captured by skis or pagat (emperor's trick) and
    // penalise the player of the mond in certain contracts
    for (int i = 0; i < trick_cards_.size(); i++) {
      if (trick_cards_.at(i) == kMondAction) {
        captured_mond_player_ = TrickCardsIndexToPlayer(i);
      }
    }
  }

  trick_cards_.clear();
  current_player_ = trick_winner;
}

TrickWinnerAndAction TarokState::ResolveTrickWinnerAndWinningAction() const {
  // compute the winning action index within trick_cards_
  int winning_action_i;
  if ((ActionInActions(kPagatAction, trick_cards_) &&
       ActionInActions(kMondAction, trick_cards_) &&
       ActionInActions(kSkisAction, trick_cards_)) &&
      (selected_contract_->name != ContractName::kColourValatWithout ||
       ActionToCard(trick_cards_.front()).suit == CardSuit::kTaroks)) {
    // the emperor trick, i.e. pagat wins over mond and skis in all cases but
    // not in Contract::kColourValatWithout when a non-trump is led
    winning_action_i =
        std::find(trick_cards_.begin(), trick_cards_.end(), kPagatAction) -
        trick_cards_.begin();
  } else {
    winning_action_i = 0;
    for (int i = 1; i < trick_cards_.size(); i++) {
      const Card& winning_card =
          ActionToCard(trick_cards_.at(winning_action_i));
      const Card& current_card = ActionToCard(trick_cards_.at(i));

      if (((current_card.suit == CardSuit::kTaroks &&
            selected_contract_->name != ContractName::kColourValatWithout) ||
           current_card.suit == winning_card.suit) &&
          current_card.rank > winning_card.rank) {
        winning_action_i = i;
      }
    }
  }
  return {TrickCardsIndexToPlayer(winning_action_i),
          trick_cards_.at(winning_action_i)};
}

open_spiel::Player TarokState::TrickCardsIndexToPlayer(int index) const {
  open_spiel::Player player = current_player_;
  for (int i = 0; i < trick_cards_.size() - 1 - index; i++) {
    player -= 1;
    if (player == -1) player = num_players_ - 1;
  }
  return player;
}

std::vector<double> TarokState::Returns() const {
  std::vector<double> returns(num_players_, 0.0);
  if (!IsTerminal()) return returns;

  std::vector<int> penalties = CapturedMondPenalties();
  std::vector<int> scores = ScoresWithoutCapturedMondPenalties();
  for (int i = 0; i < num_players_; i++) {
    returns.at(i) = penalties.at(i) + scores.at(i);
  }
  return returns;
}

std::vector<int> TarokState::CapturedMondPenalties() const {
  std::vector<int> penalties;
  penalties.reserve(num_players_);
  for (open_spiel::Player p = 0; p < num_players_; p++) {
    if (p == captured_mond_player_)
      penalties.push_back(-20);
    else
      penalties.push_back(0);
  }
  return penalties;
}

std::vector<int> TarokState::ScoresWithoutCapturedMondPenalties() const {
  if (!IsTerminal()) return std::vector<int>(num_players_, 0);
  if (selected_contract_->name == ContractName::kKlop) {
    return ScoresInKlop();
  } else if (selected_contract_->NeedsTalonExchange()) {
    return ScoresInNormalContracts();
  } else {
    // beggar and above
    return ScoresInHigherContracts();
  }
}

std::vector<int> TarokState::ScoresInKlop() const {
  std::vector<int> scores;
  scores.reserve(num_players_);

  bool any_player_won_or_lost = false;
  for (int i = 0; i < num_players_; i++) {
    int points = CardPoints(players_collected_cards_.at(i),
                            tarok_parent_game_->card_deck_);
    if (points > 35) {
      any_player_won_or_lost = true;
      scores.push_back(-70);
    } else if (points == 0) {
      any_player_won_or_lost = true;
      scores.push_back(70);
    } else {
      scores.push_back(-points);
    }
  }
  if (any_player_won_or_lost) {
    // only the winners and losers score
    for (int i = 0; i < num_players_; i++) {
      if (std::abs(scores.at(i)) != 70) scores.at(i) = 0;
    }
  }
  return scores;
}

std::vector<int> TarokState::ScoresInNormalContracts() const {
  auto [collected_cards, opposite_collected_cards] =
      SplitCollectedCardsPerTeams();
  // calculate bonuses
  int bonuses;
  if (collected_cards.size() == 48) {
    // valat won
    bonuses = 250;
  } else if (opposite_collected_cards.size() == 48) {
    // valat lost
    bonuses = -250;
  } else {
    // other bonuses that could be positive, negative or 0
    bonuses = NonValatBonuses(collected_cards, opposite_collected_cards);
  }
  // calculate final scores
  int card_points = CardPoints(collected_cards, tarok_parent_game_->card_deck_);
  int score = card_points - 35;
  if (card_points > 35)
    score += selected_contract_->score;
  else
    score -= selected_contract_->score;
  score += bonuses;

  std::vector<int> scores(num_players_, 0);
  scores.at(declarer_) = score;
  if (declarer_partner_ != open_spiel::kInvalidPlayer)
    scores.at(declarer_partner_) = score;
  return scores;
}

CollectedCardsPerTeam TarokState::SplitCollectedCardsPerTeams() const {
  std::vector<open_spiel::Action> collected_cards =
      players_collected_cards_.at(declarer_);
  std::vector<open_spiel::Action> opposite_collected_cards;
  for (open_spiel::Player p = 0; p < num_players_; p++) {
    if (p != declarer_ && p != declarer_partner_) {
      opposite_collected_cards.insert(opposite_collected_cards.end(),
                                      players_collected_cards_.at(p).begin(),
                                      players_collected_cards_.at(p).end());
    } else if (p == declarer_partner_) {
      collected_cards.insert(collected_cards.end(),
                             players_collected_cards_.at(p).begin(),
                             players_collected_cards_.at(p).end());
    }
  }
  return {collected_cards, opposite_collected_cards};
}

int TarokState::NonValatBonuses(
    const std::vector<open_spiel::Action>& collected_cards,
    const std::vector<open_spiel::Action>& opposite_collected_cards) const {
  int bonuses = 0;
  // last trick winner is the current player
  auto const& last_trick_winner_cards =
      players_collected_cards_.at(current_player_);
  // king ultimo and pagat ultimo
  int ultimo_bonus = 0;
  if (std::find(last_trick_winner_cards.end() - num_players_,
                last_trick_winner_cards.end(),
                called_king_) != last_trick_winner_cards.end()) {
    // king ultimo
    ultimo_bonus = 10;
  } else if (std::find(last_trick_winner_cards.end() - num_players_,
                       last_trick_winner_cards.end(),
                       0) != last_trick_winner_cards.end()) {
    // pagat ultimo
    ultimo_bonus = 25;
  }
  if (ultimo_bonus > 0 &&
      (current_player_ == declarer_ || current_player_ == declarer_partner_)) {
    bonuses = ultimo_bonus;
  } else if (ultimo_bonus > 0) {
    bonuses = -ultimo_bonus;
  }

  // collected kings or trula
  auto [collected_kings, collected_trula] =
      CollectedKingsAndOrTrula(collected_cards);
  auto [opposite_collected_kings, opposite_collected_trula] =
      CollectedKingsAndOrTrula(opposite_collected_cards);
  if (collected_kings)
    bonuses += 10;
  else if (opposite_collected_kings)
    bonuses -= 10;
  if (collected_trula)
    bonuses += 10;
  else if (opposite_collected_trula)
    bonuses -= 10;
  return bonuses;
}

std::tuple<bool, bool> TarokState::CollectedKingsAndOrTrula(
    const std::vector<open_spiel::Action>& collected_cards) const {
  int num_kings = 0, num_trula = 0;
  for (auto const& action : collected_cards) {
    if (action == kKingOfHeartsAction || kKingOfDiamondsAction == 37 ||
        action == kKingOfSpadesAction || kKingOfClubsAction == 53) {
      num_kings += 1;
    } else if (action == kPagatAction || action == kMondAction ||
               action == kSkisAction) {
      num_trula += 1;
    }
  }
  return {num_kings == 4, num_trula == 3};
}

std::vector<int> TarokState::ScoresInHigherContracts() const {
  bool declarer_won;
  if (selected_contract_->name == ContractName::kBeggar ||
      selected_contract_->name == ContractName::kOpenBeggar) {
    declarer_won = players_collected_cards_.at(declarer_).size() == 0;
  } else if (selected_contract_->name == ContractName::kColourValatWithout ||
             selected_contract_->name == ContractName::kValatWithout) {
    declarer_won = players_collected_cards_.at(declarer_).size() == 48;
  } else {
    // solo without
    declarer_won = CardPoints(players_collected_cards_.at(declarer_),
                              tarok_parent_game_->card_deck_) > 35;
  }

  std::vector<int> scores(num_players_, 0);
  if (declarer_won)
    scores.at(declarer_) = selected_contract_->score;
  else
    scores.at(declarer_) = -selected_contract_->score;
  return scores;
}

std::string TarokState::InformationStateString(
    open_spiel::Player player) const {
  // todo: implement
  return "";
}

std::string TarokState::ToString() const {
  std::string str = "";
  GamePhase current_game_phase = CurrentGamePhase();
  open_spiel::Player current_player = CurrentPlayer();
  absl::StrAppend(&str, "Game phase: ", GamePhaseToString(current_game_phase),
                  "\n");
  absl::StrAppend(&str, "Selected contract: ",
                  ContractNameToString(SelectedContractName()), "\n");
  absl::StrAppend(&str, "Current player: ", current_player, "\n");
  absl::StrAppend(&str, "Player cards: ",
                  absl::StrJoin(PlayerCards(current_player), ","), "\n");
  if (current_game_phase == GamePhase::kTalonExchange) {
    auto talon_sets = TalonSets();
    std::vector<std::string> talon_sets_strings;
    talon_sets_strings.reserve(talon_sets.size());
    for (auto const& set : talon_sets) {
      talon_sets_strings.push_back(absl::StrJoin(set, ","));
    }
    absl::StrAppend(
        &str, "Talon sets: ", absl::StrJoin(talon_sets_strings, ";"), "\n");
  } else if (current_game_phase == GamePhase::kTricksPlaying) {
    absl::StrAppend(&str, "Trick cards: ", absl::StrJoin(TrickCards(), ","),
                    "\n");
  }
  return str;
}

std::unique_ptr<open_spiel::State> TarokState::Clone() const {
  // todo: implement
  return nullptr;
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

const Card& TarokState::ActionToCard(open_spiel::Action action_id) const {
  return tarok_parent_game_->card_deck_.at(action_id);
}

std::ostream& operator<<(std::ostream& os, const GamePhase& game_phase) {
  os << GamePhaseToString(game_phase);
  return os;
}

std::string GamePhaseToString(const GamePhase& game_phase) {
  switch (game_phase) {
    case GamePhase::kCardDealing:
      return "Card dealing";
    case GamePhase::kBidding:
      return "Bidding";
    case GamePhase::kKingCalling:
      return "King calling";
    case GamePhase::kTalonExchange:
      return "Talon exchange";
    case GamePhase::kTricksPlaying:
      return "Tricks playing";
    case GamePhase::kFinished:
      return "Finished";
  }
}

}  // namespace tarok
