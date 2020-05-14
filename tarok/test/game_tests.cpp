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

TEST(TarokGameTests, TestCardDeckShufflingSeed) {
  auto game = tarok::NewTarokGame(
      open_spiel::GameParameters({{"rng_seed", open_spiel::GameParameter(0)}}));
  // subsequent shuffles within the same game should be different
  auto state1 = game->NewInitialTarokState();
  state1->ApplyAction(0);
  auto state2 = game->NewInitialTarokState();
  state2->ApplyAction(0);
  EXPECT_NE(state1->Talon(), state2->Talon());

  game = tarok::NewTarokGame(
      open_spiel::GameParameters({{"rng_seed", open_spiel::GameParameter(0)}}));
  // shuffles should be the same when recreating a game with the same seed
  auto state3 = game->NewInitialTarokState();
  state3->ApplyAction(0);
  auto state4 = game->NewInitialTarokState();
  state4->ApplyAction(0);
  EXPECT_EQ(state1->Talon(), state3->Talon());
  EXPECT_EQ(state2->Talon(), state4->Talon());
}

}  // namespace tarok
