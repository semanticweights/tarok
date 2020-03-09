/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <memory>
#include <string>
#include <vector>

#include "open_spiel/spiel.h"
#include "src/state.h"

namespace tarok {

// state definition
TarokState::TarokState(std::shared_ptr<const open_spiel::Game> game)
    : open_spiel::State(game) {}

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
