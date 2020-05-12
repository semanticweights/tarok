/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "open_spiel/spiel.h"
#include "src/game.h"

int main() {
  open_spiel::GameParameters params = open_spiel::GameParameters();
  std::shared_ptr<const open_spiel::Game> game = tarok::NewTarokGame(params);
  std::unique_ptr<open_spiel::State> state = game->NewInitialState();

  return 0;
}
