/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "open_spiel/spiel.h"
#include "src/game.h"
#include "src/state.h"

namespace tarok {

std::unique_ptr<TarokState> StateAfterActions(
    const open_spiel::GameParameters& params,
    const std::vector<open_spiel::Action>& actions) {
  auto state = NewTarokGame(params)->NewInitialTarokState();
  for (auto const& action : actions) {
    state->ApplyAction(action);
  }
  return state;
}

bool AllActionsInOtherActions(
    const std::vector<open_spiel::Action>& actions,
    const std::vector<open_spiel::Action>& other_actions) {
  for (const auto& action : actions) {
    if (std::find(other_actions.begin(), other_actions.end(), action) ==
        other_actions.end()) {
      return false;
    }
  }
  return true;
}

}  // namespace tarok
