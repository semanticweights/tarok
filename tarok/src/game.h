/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <memory>

#include "open_spiel/spiel.h"

namespace tarok {

// instantiate the game instance via a shared_ptr
// (see Game declaration comments in open_spiel/spiel.h)
std::shared_ptr<const open_spiel::Game> NewTarokGame(
    const open_spiel::GameParameters& params);

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
};

}  // namespace tarok
