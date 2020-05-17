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
     {"seed", open_spiel::GameParameter(kDefaultSeed)}}};

// game definition
TarokGame::TarokGame(const open_spiel::GameParameters& params)
    : Game(kGameType, params),
      num_players_(ParameterValue<int>("num_players")),
      rng_(std::mt19937(ParameterValue<int>("seed") == -1
                            ? std::time(0)
                            : ParameterValue<int>("seed"))) {}

int TarokGame::NumDistinctActions() const {
  // todo: implement
  return 0;
}

std::unique_ptr<open_spiel::State> TarokGame::NewInitialState() const {
  return NewInitialTarokState();
}

std::unique_ptr<TarokState> TarokGame::NewInitialTarokState() const {
  return std::unique_ptr<TarokState>(new TarokState(shared_from_this()));
}

int TarokGame::MaxChanceOutcomes() const {
  // game is implicitly stochastic
  return 1;
}

int TarokGame::NumPlayers() const { return num_players_; }

double TarokGame::MinUtility() const {
  // todo: implement
  return 0.0;
}

double TarokGame::MaxUtility() const {
  // todo: implement
  return 0.0;
}

std::shared_ptr<const open_spiel::Game> TarokGame::Clone() const {
  // todo: implement
  return nullptr;
}

int TarokGame::MaxGameLength() const {
  // todo: implement
  return 0;
}

int TarokGame::RNG() const { return rng_(); }

std::shared_ptr<const TarokGame> NewTarokGame(
    const open_spiel::GameParameters& params) {
  return std::shared_ptr<const TarokGame>(new TarokGame(params));
}

}  // namespace tarok
