/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gtest/gtest.h"
#include "src/game.h"

namespace tarok {

TEST(TarokGameTests, TestDefaultNumPlayers) {
  auto game = tarok::NewTarokGame(open_spiel::GameParameters());
  EXPECT_EQ(game->NumPlayers(), 3);

  game = tarok::NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(4)}}));
  EXPECT_EQ(game->NumPlayers(), 4);
}

// todo: enable and implement this test when information strings are implemented
TEST(TarokGameTests, DISABLED_TestCardDeckShufflingSeed) {
  auto game = tarok::NewTarokGame(
      open_spiel::GameParameters({{"rng_seed", open_spiel::GameParameter(0)}}));
  auto state1 = game->NewInitialTarokState();
  auto state2 = game->NewInitialTarokState();
  // subsequent shuffles within the same game should be different
  EXPECT_NE(state1, state2);

  game = tarok::NewTarokGame(
      open_spiel::GameParameters({{"rng_seed", open_spiel::GameParameter(0)}}));
  auto state3 = game->NewInitialTarokState();
  auto state4 = game->NewInitialTarokState();
  EXPECT_NE(state3, state4);

  // shuffles should be the same when recreating a game with the same seed
  EXPECT_EQ(state1, state3);
  EXPECT_EQ(state2, state4);
}

}  // namespace tarok
