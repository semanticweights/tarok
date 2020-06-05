/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <iostream>

#include "src/game.h"

int main() {
  auto game = tarok::NewTarokGame(
      open_spiel::GameParameters({{"num_players", open_spiel::GameParameter(4)},
                                  {"seed", open_spiel::GameParameter(0)}}));
  auto state = game->NewInitialTarokState();

  int selected_action;
  while (true) {
    std::cout << state->CurrentGamePhase() << std::endl;
    std::cout << state->SelectedContract() << std::endl;
    std::cout << "Current player: " << state->CurrentPlayer() << std::endl;
    std::cout << "Legal actions: ";
    auto actions = state->LegalActions();
    for (auto const& action : actions) {
      std::cout << "(" << state->ActionToString(state->CurrentPlayer(), action)
                << ", " << action << ") ";
    }
    std::cout << std::endl;
    std::cout << "Enter action: ";
    std::cin >> selected_action;
    state->ApplyAction(selected_action);
    std::cout << std::endl;
  }

  return 0;
}
