/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/game.h"

namespace tarok {

TEST(TarokStateTests, TestCardDealingPhase) {
  auto game = NewTarokGame(open_spiel::GameParameters());
  auto state = game->NewInitialTarokState();
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kCardDealing);
  EXPECT_EQ(state->CurrentPlayer(), open_spiel::kChancePlayerId);
  EXPECT_EQ(state->SelectedContract(), Contract::kNotSelected);

  EXPECT_TRUE(state->TalonSets().empty());
  for (int i = 0; i < game->NumPlayers(); i++) {
    EXPECT_TRUE(state->PlayerCards(i).empty());
  }

  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0));
  EXPECT_THAT(
      state->ChanceOutcomes(),
      testing::ElementsAreArray<open_spiel::ActionsAndProbs>({{0, 1.0}}));

  // deal the cards
  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_NE(state->CurrentPlayer(), open_spiel::kChancePlayerId);
  EXPECT_EQ(state->SelectedContract(), Contract::kNotSelected);

  // talon sets are only visible in the talon exchange phase
  EXPECT_TRUE(state->TalonSets().empty());
  EXPECT_EQ(state->Talon().size(), 6);
  for (int i = 0; i < game->NumPlayers(); i++) {
    EXPECT_FALSE(state->PlayerCards(i).empty());
  }
  EXPECT_TRUE(state->ChanceOutcomes().empty());
}

}  // namespace tarok
