/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <iostream>

#include "src/game.h"

int main() {
  auto game = tarok::NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(3)},
       {"seed", open_spiel::GameParameter(634317)}}));
  auto state = game->NewInitialTarokState();

  state->ApplyAction(0);
  state->ApplyAction(0);
  state->ApplyAction(0);
  state->ApplyAction(1);
  state->ApplyAction(1);
  std::cout << state->ToString() << std::endl;

  return 0;
}
