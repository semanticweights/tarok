/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <iostream>

#include "src/cards.h"

int main() {
  auto deck = tarok::InitializeCardDeck();
  std::vector<open_spiel::Action> card_actions(54);
  std::iota(card_actions.begin(), card_actions.end(), 0);
  double d = tarok::CardPoints(card_actions, deck);

  return 0;
}
