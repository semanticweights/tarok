/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/state.h"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "open_spiel/spiel.h"
#include "src/cards.h"
#include "src/contracts.h"
#include "src/game.h"

namespace tarok {

// state definition
TarokState::TarokState(std::shared_ptr<const open_spiel::Game> game)
    : open_spiel::State(game),
      tarok_parent_game_(std::static_pointer_cast<const TarokGame>(game)),
      current_game_phase_(GamePhase::kCardDealing) {}

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
    case GamePhase::kTalonExchange:
      // todo: implement
      return {};
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
    AddLegalActionsInBidding3(max_bid, max_bid_player, actions);
  else if (num_players_ == 4)
    AddLegalActionsInBidding4(max_bid, max_bid_player, actions);
  return actions;
}

void TarokState::AddLegalActionsInBidding3(
    int max_bid, int max_bid_player,
    std::vector<open_spiel::Action>& result_actions) const {
  result_actions.push_back(0);
  for (const int& action : kBiddableContracts3) {
    if (action < max_bid) continue;
    if ((action > max_bid) ||
        (action == max_bid && current_player_ < max_bid_player))
      result_actions.push_back(action);
  }
}

void TarokState::AddLegalActionsInBidding4(
    int max_bid, int max_bid_player,
    std::vector<open_spiel::Action>& result_actions) const {
  if (current_player_ == 0 &&
      std::all_of(players_bids_.begin() + 1, players_bids_.end(),
                  [](int i) { return i == 0; }))
    // current player is the forehand and all others have passed so it is
    // also possible to play klop or three and not possible to pass
    result_actions.insert(result_actions.end(), {1, 2});
  else
    result_actions.push_back(0);

  for (const int& action : kBiddableContracts4) {
    if (action < max_bid) continue;
    if ((action > max_bid) ||
        (action == max_bid && current_player_ < max_bid_player))
      result_actions.push_back(action);
  }
}

std::string TarokState::ActionToString(open_spiel::Player player,
                                       open_spiel::Action action_id) const {
  switch (current_game_phase_) {
    case GamePhase::kCardDealing:
      // return a dummy action due to implicit stochasticity
      return "Deal cards";
    case GamePhase::kBidding:
      if (action_id == 0)
        return "Pass";
      else
        return tarok_parent_game_->Contract(action_id).name;
    case GamePhase::kTalonExchange:
    case GamePhase::kTricksPlaying:
      return tarok_parent_game_->Card(action_id).ToString();
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
  if (current_game_phase_ == GamePhase::kCardDealing)
    // return a dummy action with probability 1 due to implicit stochasticity
    return {{0, 1.0}};
  return {};
}

GamePhase TarokState::CurrentGamePhase() const { return current_game_phase_; }

std::vector<std::string> TarokState::Talon() const {
  std::vector<std::string> talon;
  talon.reserve(talon_.size());

  for (const int& card_index : talon_) {
    talon.push_back(tarok_parent_game_->Card(card_index).ToString());
  }
  return talon;
}

std::vector<std::string> TarokState::PlayerCards(
    open_spiel::Player player) const {
  if (current_game_phase_ == GamePhase::kCardDealing) return {};

  std::vector<std::string> player_cards;
  player_cards.reserve(players_cards_.at(player).size());

  for (const int& card_index : players_cards_.at(player)) {
    player_cards.push_back(tarok_parent_game_->Card(card_index).ToString());
  }
  return player_cards;
}

void TarokState::DoApplyAction(open_spiel::Action action_id) {
  // todo: we should probably check that action is legal for the current player
  switch (current_game_phase_) {
    case GamePhase::kCardDealing:
      DoApplyActionInCardDealing();
      break;
    case GamePhase::kBidding:
      // todo: implement
      break;
    case GamePhase::kTalonExchange:
      // todo: implement
      break;
    case GamePhase::kTricksPlaying:
      // todo: implement
      break;
    case GamePhase::kFinished:
      open_spiel::SpielFatalError("Calling DoApplyAction on a terminal state.");
  }
}

void TarokState::NextPlayer() {
  current_player_ += 1;
  if (current_player_ == num_players_) current_player_ = 0;
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
  else if (num_players_ == 4)
    // forehand has to wait
    current_player_ = 1;
}

}  // namespace tarok
