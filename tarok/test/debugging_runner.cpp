/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <iostream>

#include <array>
#include <vector>

#include <algorithm>
#include <iterator>
#include <random>

#include "src/cards.h"
#include "src/game.h"
#include "src/state.h"

int main() {
  open_spiel::GameParameters params = open_spiel::GameParameters();
  std::shared_ptr<const tarok::TarokGame> game = tarok::NewTarokGame(params);
  std::unique_ptr<open_spiel::State> base_state = game->NewInitialState();
  std::unique_ptr<tarok::TarokState> state(
      static_cast<tarok::TarokState*>(base_state.release()));

  std::cout << "TALON:" << std::endl;
  for (auto c : state->talon_) {
    std::cout << game->kDeck[c].ToString() << std::endl;
  }
  std::cout << std::endl;

  for (int i = 0; i < state->private_cards_.size(); i++) {
    std::cout << "PLAYER " << i << ":" << std::endl;
    for (auto c : state->private_cards_[i]) {
      std::cout << game->kDeck[c].ToString() << std::endl;
    }
    std::cout << std::endl;
  }

  return 0;
}
