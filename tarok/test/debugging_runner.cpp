/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <iostream>

#include "open_spiel/spiel.h"
#include "src/game.h"

int main() {
  auto game = tarok::NewTarokGame(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(0)}}));
  auto state = game->NewInitialTarokState();

  state->ApplyAction(0);
  state->LegalActions();

  return 0;
}
