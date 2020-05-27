/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/state.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "open_spiel/abseil-cpp/absl/strings/str_cat.h"
#include "open_spiel/spiel.h"
#include "src/cards.h"
#include "src/contracts.h"
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
      // todo: implement
      return {};
    case GamePhase::kFinished:
      return {};
  }
}

std::vector<open_spiel::Action> TarokState::LegalActionsInBidding() const {
  // actions 1 - 12 correspond to contracts as returned by InitializeContracts()
  // respectively, action 0 means pass
  auto it = std::max_element(players_bids_.begin(), players_bids_.end());
  int max_bid = *it;
  int max_bid_player = it - players_bids_.begin();

  std::vector<open_spiel::Action> actions;
  if (num_players_ == 3)
    AddLegalActionsInBidding3(max_bid, max_bid_player, &actions);
  else
    AddLegalActionsInBidding4(max_bid, max_bid_player, &actions);
  return actions;
}

void TarokState::AddLegalActionsInBidding3(
    int max_bid, int max_bid_player,
    std::vector<open_spiel::Action>* result_actions) const {
  if (!AllButCurrentPlayerPassedBidding() ||
      (current_player_ == 2 && players_bids_.at(current_player_) == -1)) {
    // other players still playing or the last player and no bidding has
    // happened before which results in a compulsory klop
    result_actions->push_back(0);
  }

  for (const int& action : kBiddableContracts3) {
    if (action < max_bid) continue;
    if ((action > max_bid) ||
        (action == max_bid && current_player_ <= max_bid_player)) {
      result_actions->push_back(action);
    }
  }
}

void TarokState::AddLegalActionsInBidding4(
    int max_bid, int max_bid_player,
    std::vector<open_spiel::Action>* result_actions) const {
  if (current_player_ == 0 && players_bids_.at(current_player_) == -1 &&
      AllButCurrentPlayerPassedBidding()) {
    // no bidding has happened before so forehand can
    // bid any contract but can't pass
    result_actions->insert(result_actions->end(), {1, 2});
  } else if (!AllButCurrentPlayerPassedBidding()) {
    // other players still playing
    result_actions->push_back(0);
  }

  for (const int& action : kBiddableContracts4) {
    if (action < max_bid) continue;
    if ((action > max_bid) ||
        (action == max_bid && current_player_ <= max_bid_player)) {
      result_actions->push_back(action);
    }
  }
}

std::vector<open_spiel::Action> TarokState::LegalActionsInTalonExchange()
    const {
  if (talon_.size() == 6) {
    // choosing one of the talon card sets where actions are encoded as
    // 0, 1, 2, etc. from left to right, i.e. 0 is the leftmost talon set
    std::vector<open_spiel::Action> actions(
        6 / selected_contract_->num_talon_exchanges);
    std::iota(actions.begin(), actions.end(), 0);
    return actions;
  }

  // discarding the cards
  std::vector<open_spiel::Action> actions;
  for (const auto& card : players_cards_.at(current_player_)) {
    if (tarok_parent_game_->card_deck_.at(card).points != 5)
      actions.push_back(card);
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

open_spiel::Action TarokState::StringToAction(
    open_spiel::Player player, const std::string& action_str) const {
  // todo: implement
  return 0;
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

std::vector<std::vector<open_spiel::Action>> TarokState::Talon() const {
  if (current_game_phase_ != GamePhase::kTalonExchange) return {};

  int num_talon_sets = talon_.size() / selected_contract_->num_talon_exchanges;
  std::vector<std::vector<open_spiel::Action>> talon;
  talon.reserve(num_talon_sets);

  for (int i = 0; i < num_talon_sets; i++) {
    auto begin = talon_.begin() + i * selected_contract_->num_talon_exchanges;
    talon.push_back(std::vector<open_spiel::Action>(
        begin, begin + selected_contract_->num_talon_exchanges));
  }
  return talon;
}

std::vector<open_spiel::Action> TarokState::PlayerCards(
    open_spiel::Player player) const {
  if (current_game_phase_ == GamePhase::kCardDealing) return {};
  return players_cards_.at(player);
}

std::string TarokState::CardActionToString(open_spiel::Action action_id) const {
  return tarok_parent_game_->card_deck_.at(action_id).ToString();
}

Contract TarokState::SelectedContract() const {
  if (current_game_phase_ == GamePhase::kCardDealing ||
      current_game_phase_ == GamePhase::kBidding) {
    return Contract::kNotSelected;
  }
  return selected_contract_->contract;
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
      // todo: implement
      break;
    case GamePhase::kFinished:
      open_spiel::SpielFatalError("Calling DoApplyAction on a terminal state.");
  }
}

void TarokState::DoApplyActionInCardDealing() {
  // do the actual sampling here due to implicit stochasticity
  // todo: deal again if any player without taroks and play compulsory klop
  std::tie(talon_, players_cards_) =
      DealCards(num_players_, tarok_parent_game_->RNG());

  current_game_phase_ = GamePhase::kBidding;
  players_bids_.reserve(num_players_);
  players_bids_.insert(players_bids_.end(), num_players_, -1);

  // lower player indices correspond to higher bidding priority,
  // i.e. 0 is the forehand, num_players - 1 is the dealer
  if (num_players_ == 3)
    current_player_ = 0;
  else
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

void TarokState::FinishBiddingPhase(open_spiel::Action action_id) {
  declarer_ = current_player_;
  if (action_id == 0) {
    // compulsory klop, this state should only occur in a 3-player game
    selected_contract_ = &tarok_parent_game_->contracts_.at(0);
  } else {
    selected_contract_ = &tarok_parent_game_->contracts_.at(action_id - 1);
  }

  if (num_players_ == 4 && selected_contract_->needs_king_calling)
    current_game_phase_ = GamePhase::kKingCalling;
  else if (selected_contract_->NeedsTalonExchange())
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
  if (talon_.size() == 6) {
    // choosing one of the talon card sets
    int pos = action_id * selected_contract_->num_talon_exchanges;
    for (int i = pos; i < pos + selected_contract_->num_talon_exchanges; i++) {
      players_cards_.at(current_player_).push_back(talon_.at(i));
    }
    std::sort(players_cards_.at(current_player_).begin(),
              players_cards_.at(current_player_).end());
    talon_.erase(
        talon_.begin() + pos,
        talon_.begin() + pos + selected_contract_->num_talon_exchanges);
  } else {
    // discarding the cards
    MoveActionFromTo(action_id, &players_cards_.at(current_player_),
                     &players_collected_cards_.at(current_player_));
    if (players_cards_.at(current_player_).size() == 48 / num_players_) {
      StartTricksPlayingPhase();
    }
  }
}

void TarokState::StartTricksPlayingPhase() {
  current_game_phase_ = GamePhase::kTricksPlaying;
  if (!selected_contract_->declarer_starts) current_player_ = 0;
}

bool TarokState::AllButCurrentPlayerPassedBidding() const {
  for (int i = 0; i < num_players_; i++) {
    if (i == current_player_) continue;
    if (players_bids_.at(i) != 0) return false;
  }
  return true;
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

}  // namespace tarok
