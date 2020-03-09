/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <memory>

#include "open_spiel/spiel.h"
#include "src/cards.h"

namespace tarok {

class TarokGame : public open_spiel::Game {
 public:
  explicit TarokGame(const open_spiel::GameParameters& params);

  int NumDistinctActions() const override;
  std::unique_ptr<open_spiel::State> NewInitialState() const override;
  // int MaxChanceOutcomes() const override;
  int NumPlayers() const override;
  double MinUtility() const override;
  double MaxUtility() const override;
  std::shared_ptr<const Game> Clone() const override;
  // double UtilitySum() const override;
  int MaxGameLength() const override;

  const std::array<Card, 54> kDeck = BuildDeck();
};

// instantiate the game instance via a shared_ptr
// (see Game declaration comments in open_spiel/spiel.h)
std::shared_ptr<const TarokGame> NewTarokGame(
    const open_spiel::GameParameters& params);

}  // namespace tarok
