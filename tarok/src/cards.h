/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <array>
#include <string>
#include <tuple>
#include <vector>

namespace tarok {

enum class Suit { kHearts, kDiamonds, kSpades, kClubs, kTaroks };

class Card {
 public:
  explicit Card(Suit suit, int rank, int points, std::string short_name,
                std::string long_name);
  bool IsTrula() const;
  std::string ToString() const;

  const Suit suit_;
  const int rank_;
  const int points_;
  const std::string short_name_;
  const std::string long_name_;
};

std::array<Card, 54> BuildDeck();

std::tuple<std::vector<int>, std::array<std::vector<int>, 3>> DealCards(
    int seed);

}  // namespace tarok
