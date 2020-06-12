/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <iostream>

#include "src/game.h"

int main() {
  auto game = tarok::NewTarokGame(
      open_spiel::GameParameters({{"num_players", open_spiel::GameParameter(3)},
                                  {"seed", open_spiel::GameParameter(0)}}));
  auto state = game->NewInitialTarokState();

  state->ApplyAction(0);
  state->ApplyAction(0);
  state->ApplyAction(0);
  state->ApplyAction(1);
  state->ApplyAction(3);
  state->ApplyAction(5);

  return 0;
}
