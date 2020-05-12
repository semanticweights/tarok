/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/game.h"

#include <ctime>
#include <memory>

#include "open_spiel/spiel.h"
#include "src/cards.h"
#include "src/state.h"

namespace tarok {

// game facts
const open_spiel::GameType kGameType{
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
     {"rng_seed", open_spiel::GameParameter(kDefaultRngSeed)}}};

// game definition
TarokGame::TarokGame(const open_spiel::GameParameters& params)
    : Game(kGameType, params),
      num_players_(this->ParameterValue<int>("num_players")),
      rng_(new std::mt19937(this->ParameterValue<int>("rng_seed") == -1
                                ? std::time(0)
                                : this->ParameterValue<int>("rng_seed"))) {}

int TarokGame::NumDistinctActions() const { return 0; }

std::unique_ptr<open_spiel::State> TarokGame::NewInitialState() const {
  return NewInitialTarokState();
}

std::unique_ptr<TarokState> TarokGame::NewInitialTarokState() const {
  return std::unique_ptr<TarokState>(new TarokState(shared_from_this()));
}

int TarokGame::NumPlayers() const { return num_players_; }

double TarokGame::MinUtility() const { return 0.0; }

double TarokGame::MaxUtility() const { return 0.0; }

std::shared_ptr<const open_spiel::Game> TarokGame::Clone() const {
  return nullptr;
}

int TarokGame::MaxGameLength() const { return 0; }

int TarokGame::ShuffleCardDeckSeed() const { return rng_->operator()(); }

TarokCard TarokGame::ActionToCard(open_spiel::Action action) const {
  return kCardDeck[action];
}

std::shared_ptr<const TarokGame> NewTarokGame(
    const open_spiel::GameParameters& params) {
  return std::shared_ptr<const TarokGame>(new TarokGame(params));
}

}  // namespace tarok
