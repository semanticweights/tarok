/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <array>
#include <string>
#include <tuple>
#include <vector>

#include "open_spiel/spiel.h"

namespace tarok {

enum class CardSuit { kHearts, kDiamonds, kSpades, kClubs, kTaroks };

struct TarokCard {
  TarokCard(CardSuit suit, int rank, int points, std::string short_name,
            std::string long_name);

  bool IsTrula() const;
  const std::string ToString() const;

  const CardSuit suit;
  const int rank;
  const int points;
  const std::string short_name;
  const std::string long_name;
};

const std::array<TarokCard, 54> InitializeCardDeck();

// a type for a pair holding talon and players' private cards
using DealtCards = std::tuple<std::vector<open_spiel::Action>,
                              std::vector<std::vector<open_spiel::Action>>>;
DealtCards DealCards(int num_players, int seed);

// we use our own implementation since std::shuffle is non-deterministic across
// different versions of the standard library implementation
void Shuffle(std::vector<open_spiel::Action>* actions, std::mt19937&& rng);

}  // namespace tarok
