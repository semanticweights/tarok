/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <memory>

#include "open_spiel/spiel.h"
#include "src/game.h"

namespace tarok {

std::shared_ptr<const open_spiel::Game> NewTarokGame(
    const open_spiel::GameParameters& params) {
  return std::shared_ptr<const open_spiel::Game>(new TarokGame(params));
}

// game facts
const open_spiel::GameType kGameType{
    "tarok",            // short_name
    "Slovenian Tarok",  // long_name
    open_spiel::GameType::Dynamics::kSequential,
    open_spiel::GameType::ChanceMode::kExplicitStochastic,
    open_spiel::GameType::Information::kImperfectInformation,
    open_spiel::GameType::Utility::kGeneralSum,
    open_spiel::GameType::RewardModel::kTerminal,
    3,      // max_num_players
    3,      // min_num_players
    true,   // provides_information_state_string
    false,  // provides_information_state_tensor
    false,  // provides_observation_string
    false,  // provides_observation_tensor
    {}      // parameter_specification
};

// game definition
TarokGame::TarokGame(const open_spiel::GameParameters& params)
    : Game(kGameType, params) {}

int TarokGame::NumDistinctActions() const { return 0; }

std::unique_ptr<open_spiel::State> TarokGame::NewInitialState() const {
  return nullptr;
}

int TarokGame::NumPlayers() const { return 3; }

double TarokGame::MinUtility() const { return 0.0; }

double TarokGame::MaxUtility() const { return 0.0; }

std::shared_ptr<const open_spiel::Game> TarokGame::Clone() const {
  return nullptr;
}

int TarokGame::MaxGameLength() const { return 0; }

}  // namespace tarok