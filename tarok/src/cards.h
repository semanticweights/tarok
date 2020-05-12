/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <string>
#include <tuple>
#include <vector>

namespace tarok {

enum class CardSuit { kHearts, kDiamonds, kSpades, kClubs, kTaroks };

struct TarokCard {
  TarokCard(CardSuit suit, int rank, int points, std::string short_name,
            std::string long_name);

  bool IsTrula() const;
  std::string ToString() const;

  const CardSuit suit_;
  const int rank_;
  const int points_;
  const std::string short_name_;
  const std::string long_name_;
};

using CardDeck = std::array<TarokCard, 54>;
CardDeck InitializeCardDeck();

// a type for a pair holding talon and players' private cards
using DealtCards = std::tuple<std::vector<int>, std::vector<std::vector<int>>>;
DealtCards DealCards(int numPlayers, int seed);

}  // namespace tarok
