/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "open_spiel/spiel.h"
#include "src/game.h"

namespace tarok {

TEST(TarokStateTests, TestCardDealingPhase) {
  auto game = tarok::NewTarokGame(open_spiel::GameParameters());
  auto state = game->NewInitialTarokState();
  EXPECT_EQ(state->CurrentPlayer(), open_spiel::kChancePlayerId);
  EXPECT_TRUE(state->Talon().empty());
  for (int i = 0; i < game->NumPlayers(); i++) {
    EXPECT_TRUE(state->PlayerCards(i).empty());
  }
  EXPECT_EQ(state->LegalActions().size(), 1);
  ASSERT_THAT(state->LegalActions(), testing::ElementsAre(0));

  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_TRUE(!state->Talon().empty());
  for (int i = 0; i < game->NumPlayers(); i++) {
    EXPECT_TRUE(!state->PlayerCards(i).empty());
  }
}

}  // namespace tarok
