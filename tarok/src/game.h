/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <array>
#include <memory>

#include "open_spiel/spiel.h"
#include "src/cards.h"
#include "src/state.h"

namespace tarok {

inline constexpr int kDefaultNumPLayers = 3;
// seed for shuffling the cards, -1 means seeded by clock
inline constexpr int kDefaultSeed = -1;

// game facts
inline static const open_spiel::GameType kGameType{
    "tarok",            // short_name
    "Slovenian Tarok",  // long_name
    open_spiel::GameType::Dynamics::kSequential,
    open_spiel::GameType::ChanceMode::kSampledStochastic,
    open_spiel::GameType::Information::kImperfectInformation,
    open_spiel::GameType::Utility::kGeneralSum,
    open_spiel::GameType::RewardModel::kTerminal,
    4,      // max_num_players
    3,      // min_num_players
    true,   // provides_information_state_string
    false,  // provides_information_state_tensor
    false,  // provides_observation_string
    false,  // provides_observation_tensor
    // parameter_specification
    {{"num_players", open_spiel::GameParameter(kDefaultNumPLayers)},
     {"seed", open_spiel::GameParameter(kDefaultSeed)}}};

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

  inline static const std::array<TarokCard, 54> card_deck_ =
      InitializeCardDeck();
  const int num_players_;
  mutable std::mt19937 rng_;
};

// instantiate the game instance via a shared_ptr, see game declaration
// comments in open_spiel/spiel.h for more info
std::shared_ptr<const TarokGame> NewTarokGame(
    const open_spiel::GameParameters& params);

}  // namespace tarok
