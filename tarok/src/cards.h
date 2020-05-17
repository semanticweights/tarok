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

  const CardSuit suit;
  const int rank;
  const int points;
  const std::string short_name;
  const std::string long_name;
};

std::array<TarokCard, 54> InitializeCardDeck();

// a type for a pair holding talon and players' private cards
using DealtCards = std::tuple<std::vector<int>, std::vector<std::vector<int>>>;
DealtCards DealCards(int num_players, int seed);

}  // namespace tarok
