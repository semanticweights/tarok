/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <iostream>

#include "src/game.h"

int main() {
  open_spiel::GameParameters params = open_spiel::GameParameters();
  std::shared_ptr<const tarok::TarokGame> game = tarok::NewTarokGame(params);

  for (auto c : game->kDeck) {
    std::cout << c.ToString() << std::endl;
  }

  return 0;
}
