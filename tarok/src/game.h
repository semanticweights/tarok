/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <memory>

#include "open_spiel/spiel.h"
#include "src/cards.h"
#include "src/state.h"

namespace tarok {

inline constexpr int kDefaultNumPLayers = 3;
// seed for shuffling the cards, should be >= 0, -1 means seeded by clock
inline constexpr int kDefaultRngSeed = -1;

class TarokGame : public open_spiel::Game {
 public:
  explicit TarokGame(const open_spiel::GameParameters& params);

  int NumDistinctActions() const override;
  std::unique_ptr<open_spiel::State> NewInitialState() const override;
  std::unique_ptr<TarokState> NewInitialTarokState() const;
  // int MaxChanceOutcomes() const override;
  int NumPlayers() const override;
  double MinUtility() const override;
  double MaxUtility() const override;
  std::shared_ptr<const Game> Clone() const override;
  // double UtilitySum() const override;
  int MaxGameLength() const override;
  int ShuffleCardDeckSeed() const;
  TarokCard ActionToCard(open_spiel::Action action) const;

 private:
  inline static const CardDeck kCardDeck = InitializeCardDeck();
  int num_players_;
  std::unique_ptr<std::mt19937> rng_;
};

// instantiate the game instance via a shared_ptr
// (see Game declaration comments in open_spiel/spiel.h)
std::shared_ptr<const open_spiel::Game> NewGame(
    const open_spiel::GameParameters& params);

std::shared_ptr<const TarokGame> NewTarokGame(
    const open_spiel::GameParameters& params);

}  // namespace tarok
