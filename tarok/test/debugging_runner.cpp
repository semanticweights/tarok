/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "open_spiel/spiel.h"
#include "src/game.h"

int main() {
  auto params =
      open_spiel::GameParameters({{"rng_seed", open_spiel::GameParameter(0)}});
  auto game = tarok::NewTarokGame(params);
  auto state1 = game->NewInitialTarokState();
  auto state2 = game->NewInitialTarokState();

  return 0;
}
