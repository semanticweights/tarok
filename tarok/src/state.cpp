/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/state.h"

#include <memory>
#include <string>
#include <vector>

#include "open_spiel/spiel.h"
#include "src/game.h"

namespace tarok {

// state definition
TarokState::TarokState(std::shared_ptr<const open_spiel::Game> game)
    : open_spiel::State(game),
      tarok_parent_game_(static_cast<const TarokGame&>(*game)) {
  std::tie(talon_, players_cards) =
      DealCards(game->NumPlayers(), tarok_parent_game_.RngSeed());
}

open_spiel::Player TarokState::CurrentPlayer() const { return -1; }

std::vector<open_spiel::Action> TarokState::LegalActions() const {
  return std::vector<open_spiel::Action>{};
}

std::string TarokState::ActionToString(open_spiel::Player player,
                                       open_spiel::Action action_id) const {
  return "";
}

open_spiel::Action TarokState::StringToAction(
    open_spiel::Player player, const std::string& action_str) const {
  return open_spiel::Action();
}

std::string TarokState::ToString() const { return ""; }

bool TarokState::IsTerminal() const { return true; }

std::vector<double> TarokState::Returns() const {
  return std::vector<double>{};
}

std::string TarokState::InformationStateString(
    open_spiel::Player player) const {
  return "";
}

std::unique_ptr<open_spiel::State> TarokState::Clone() const { return nullptr; }

open_spiel::ActionsAndProbs TarokState::ChanceOutcomes() const {
  return open_spiel::ActionsAndProbs();
}

void TarokState::DoApplyAction(open_spiel::Action action_id) {}

}  // namespace tarok
