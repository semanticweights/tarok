/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/game.h"

#include <ctime>

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

int TarokGame::NumDistinctActions() const { return 54; }

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

double TarokGame::MinUtility() const { return -500.0; }

double TarokGame::MaxUtility() const { return 500.0; }

std::shared_ptr<const open_spiel::Game> TarokGame::Clone() const {
  return std::shared_ptr<const open_spiel::Game>(new TarokGame(*this));
}

int TarokGame::MaxGameLength() const {
  // the longest round is when players bid the lowest possible contract each
  // time (18 or 14 moves, depending on num_players_) and then each has to play
  // the 12 or 16 cards in hand
  return 30;
}

int TarokGame::RNG() const { return rng_(); }

std::shared_ptr<const TarokGame> NewTarokGame(
    const open_spiel::GameParameters& params) {
  return std::make_shared<const TarokGame>(params);
}

open_spiel::REGISTER_SPIEL_GAME(kGameType, NewTarokGame);

}  // namespace tarok
