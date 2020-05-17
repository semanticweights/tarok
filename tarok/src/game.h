/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <memory>

#include "open_spiel/spiel.h"
#include "src/cards.h"
#include "src/state.h"

namespace tarok {

inline constexpr int kDefaultNumPLayers = 3;
// seed for shuffling the cards, -1 means seeded by clock
inline constexpr int kDefaultSeed = -1;

class TarokGame : public open_spiel::Game {
 public:
  explicit TarokGame(const open_spiel::GameParameters& params);

  int NumDistinctActions() const override;
  std::unique_ptr<open_spiel::State> NewInitialState() const override;
  std::unique_ptr<TarokState> NewInitialTarokState() const;
  int MaxChanceOutcomes() const override;
  int NumPlayers() const override;
  double MinUtility() const override;
  double MaxUtility() const override;
  std::shared_ptr<const Game> Clone() const override;
  int MaxGameLength() const override;

 private:
  friend class TarokState;
  // this function is const so that it can be called from state instances,
  // note that it nevertheless changes the state of the mutable rng_ used
  // for shuffling the cards
  int RNG() const;

  inline static const CardDeck kCardDeck = InitializeCardDeck();
  const int num_players_;
  mutable std::mt19937 rng_;
};

// instantiate the game instance via a shared_ptr, see game declaration
// comments in open_spiel/spiel.h for more info
std::shared_ptr<const TarokGame> NewTarokGame(
    const open_spiel::GameParameters& params);

}  // namespace tarok
