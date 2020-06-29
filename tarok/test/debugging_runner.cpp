/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <iostream>

#include "src/game.h"

int main() {
  auto game = tarok::NewTarokGame(open_spiel::GameParameters({}));
  game->NewInitialTarokState()->ApplyAction(0);
  auto cloned_game = game->Clone();

  auto state0 = game->NewInitialTarokState();
  state0->ApplyAction(0);
  auto state1 = cloned_game->NewInitialState();
  state1->ApplyAction(0);

  return 0;
}
