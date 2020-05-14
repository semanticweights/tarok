/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <iostream>

#include "open_spiel/spiel.h"
#include "src/game.h"

int main() {
  auto game = tarok::NewTarokGame(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(0)}}));

  auto state = game->NewInitialTarokState();
  std::cout << "Current player: " << state->CurrentPlayer() << std::endl;
  std::cout << "Talon size: " << state->Talon().size() << "\n" << std::endl;

  std::cout << "Dealing cards..." << std::endl;
  state->ApplyAction(0);
  std::cout << "Current player: " << state->CurrentPlayer() << std::endl;
  std::cout << "Talon size: " << state->Talon().size() << std::endl;

  return 0;
}
