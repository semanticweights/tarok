/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <array>
#include <string>
#include <tuple>
#include <vector>

#include "open_spiel/spiel.h"

namespace tarok {

// a subset of cards actions that are used throughout the
// codebase and add to readability
static constexpr int kPagatAction = 0;
static constexpr int kMondAction = 20;
static constexpr int kSkisAction = 21;
static constexpr int kKingOfHeartsAction = 29;
static constexpr int kKingOfDiamondsAction = 37;
static constexpr int kKingOfSpadesAction = 45;
static constexpr int kKingOfClubsAction = 53;

enum class CardSuit { kHearts, kDiamonds, kSpades, kClubs, kTaroks };

struct Card {
  Card(CardSuit suit, int rank, int points, std::string short_name,
       std::string long_name);

  const std::string ToString() const;

  const CardSuit suit;
  const int rank;
  const int points;
  const std::string short_name;
  const std::string long_name;
};

const std::array<Card, 54> InitializeCardDeck();

// a type for a pair holding talon and players' private cards
using DealtCards = std::tuple<std::vector<open_spiel::Action>,
                              std::vector<std::vector<open_spiel::Action>>>;
DealtCards DealCards(int num_players, int seed);

// we use our own implementation since std::shuffle is non-deterministic across
// different versions of the standard library implementation
void Shuffle(std::vector<open_spiel::Action>* actions, std::mt19937&& rng);

int CardPoints(const std::vector<open_spiel::Action>& actions,
               const std::array<Card, 54>& deck);

}  // namespace tarok
