/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <iostream>

#include "src/game.h"

int main() {
  auto game = tarok::NewTarokGame(open_spiel::GameParameters({}));
  auto state = game->NewInitialTarokState();
  state->ApplyAction(0);
  state->ApplyAction(0);
  state->ApplyAction(0);

  return 0;
}
