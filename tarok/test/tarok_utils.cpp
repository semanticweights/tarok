/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "test/tarok_utils.h"

#include <algorithm>

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
  for (auto const& action : actions) {
    if (std::find(other_actions.begin(), other_actions.end(), action) ==
        other_actions.end()) {
      return false;
    }
  }
  return true;
}

open_spiel::Action CardLongNameToAction(const std::string& long_name,
                                        const std::array<Card, 54>& deck) {
  for (int i = 0; i < deck.size(); i++) {
    if (deck.at(i).long_name == long_name) return i;
  }
  open_spiel::SpielFatalError("Invalid long_name!");
  return -1;
}

std::vector<open_spiel::Action> CardLongNamesToActions(
    const std::vector<std::string>& long_names,
    const std::array<Card, 54>& deck) {
  std::vector<open_spiel::Action> actions;
  actions.reserve(long_names.size());
  for (auto const long_name : long_names) {
    actions.push_back(CardLongNameToAction(long_name, deck));
  }
  return actions;
}

}  // namespace tarok
