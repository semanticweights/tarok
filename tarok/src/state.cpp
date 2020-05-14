/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/state.h"

#include <memory>
#include <string>
#include <vector>

#include "open_spiel/spiel.h"
#include "src/cards.h"
#include "src/game.h"

namespace tarok {

// state definition
TarokState::TarokState(std::shared_ptr<const open_spiel::Game> game)
    : open_spiel::State(game),
      tarok_parent_game_(static_cast<const TarokGame&>(*game)),
      current_game_phase_(GamePhase::kCardDealing),
      current_player_(open_spiel::kChancePlayerId) {}

open_spiel::Player TarokState::CurrentPlayer() const { return current_player_; }

std::vector<open_spiel::Action> TarokState::LegalActions() const {
  switch (current_game_phase_) {
    case GamePhase::kCardDealing:
      // return a dummy action due to implicit stochasticity
      return {0};
    case GamePhase::kBidding:
      // todo: implement
      return {};
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

std::string TarokState::ActionToString(open_spiel::Player player,
                                       open_spiel::Action action_id) const {
  // todo: implement
  return "";
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

void TarokState::DoApplyActionInCardDealing() {
  // do the actual sampling here due to implicit stochasticity
  std::tie(talon_, players_cards_) =
      DealCards(tarok_parent_game_.NumPlayers(),
                tarok_parent_game_.ShuffleCardDeckSeed());
  current_game_phase_ = GamePhase::kBidding;
  current_player_ = 0;
}

}  // namespace tarok
