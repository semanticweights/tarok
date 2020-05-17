/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/game.h"

#include <ctime>
#include <memory>

#include "open_spiel/spiel.h"
#include "src/cards.h"
#include "src/state.h"

namespace tarok {

TarokGame::TarokGame(const open_spiel::GameParameters& params)
    : Game(kGameType, params),
      num_players_(ParameterValue<int>("num_players")),
      rng_(std::mt19937(ParameterValue<int>("seed") == -1
                            ? std::time(0)
                            : ParameterValue<int>("seed"))) {
  SPIEL_CHECK_GE(num_players_, kGameType.min_num_players);
  SPIEL_CHECK_LE(num_players_, kGameType.max_num_players);
}

int TarokGame::NumDistinctActions() const {
  // todo: implement
  return 0;
}

std::unique_ptr<open_spiel::State> TarokGame::NewInitialState() const {
  return NewInitialTarokState();
}

std::unique_ptr<TarokState> TarokGame::NewInitialTarokState() const {
  return std::make_unique<TarokState>(shared_from_this());
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

const TarokCard& TarokGame::Card(int card_index) const {
  return card_deck_.at(card_index);
}

std::shared_ptr<const TarokGame> NewTarokGame(
    const open_spiel::GameParameters& params) {
  return std::make_shared<const TarokGame>(params);
}

}  // namespace tarok
