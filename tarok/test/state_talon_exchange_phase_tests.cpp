/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "test/tarok_utils.h"

namespace tarok {

TEST(TarokStateTests, TestTalonExchangePhase1) {
  // 3 talon exchanges, select the first set
  auto state = StateAfterActions(open_spiel::GameParameters(), {0, 0, 0, 2});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContract(), Contract::kThree);
  auto talon_initial = state->TalonSets();
  EXPECT_EQ(talon_initial.size(), 2);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 1));
  for (const auto& talon_set : talon_initial) {
    EXPECT_EQ(talon_set.size(), 3);
  }

  // select the first set
  state->ApplyAction(0);
  auto talon_end = state->TalonSets();
  EXPECT_EQ(talon_end.size(), 1);
  EXPECT_EQ(talon_initial.at(1), talon_end.at(0));
  EXPECT_TRUE(AllActionsInOtherActions(
      talon_initial.at(0), state->PlayerCards(state->CurrentPlayer())));

  // discard the first three cards
  auto legal_actions = state->LegalActions();
  for (int i = 0; i < 3; i++) {
    state->ApplyAction(legal_actions.at(i));
    EXPECT_FALSE(AllActionsInOtherActions(
        {legal_actions.at(i)}, state->PlayerCards(state->CurrentPlayer())));
  }
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
}

TEST(TarokStateTests, TestTalonExchangePhase2) {
  // 3 talon exchanges, select the second set
  auto state = StateAfterActions(open_spiel::GameParameters(), {0, 0, 0, 2});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContract(), Contract::kThree);
  auto talon_initial = state->TalonSets();
  EXPECT_EQ(talon_initial.size(), 2);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 1));
  for (const auto& talon_set : talon_initial) {
    EXPECT_EQ(talon_set.size(), 3);
  }

  // select the second set
  state->ApplyAction(1);
  auto talon_end = state->TalonSets();
  EXPECT_EQ(talon_end.size(), 1);
  EXPECT_EQ(talon_initial.at(0), talon_end.at(0));
  EXPECT_TRUE(AllActionsInOtherActions(
      talon_initial.at(1), state->PlayerCards(state->CurrentPlayer())));

  // discard the first three cards
  auto legal_actions = state->LegalActions();
  for (int i = 0; i < 3; i++) {
    state->ApplyAction(legal_actions.at(i));
    EXPECT_FALSE(AllActionsInOtherActions(
        {legal_actions.at(i)}, state->PlayerCards(state->CurrentPlayer())));
  }
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
}

TEST(TarokStateTests, TestTalonExchangePhase3) {
  // 2 talon exchanges, select the middle set
  auto state = StateAfterActions(open_spiel::GameParameters(), {0, 0, 0, 3});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContract(), Contract::kTwo);
  auto talon_initial = state->TalonSets();
  EXPECT_EQ(talon_initial.size(), 3);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 1, 2));
  for (const auto& talon_set : talon_initial) {
    EXPECT_EQ(talon_set.size(), 2);
  }

  // select the middle set
  state->ApplyAction(1);
  auto talon_end = state->TalonSets();
  EXPECT_EQ(talon_end.size(), 2);
  EXPECT_EQ(talon_initial.at(0), talon_end.at(0));
  EXPECT_EQ(talon_initial.at(2), talon_end.at(1));
  EXPECT_TRUE(AllActionsInOtherActions(
      talon_initial.at(1), state->PlayerCards(state->CurrentPlayer())));

  // discard the first two cards
  auto legal_actions = state->LegalActions();
  for (int i = 0; i < 2; i++) {
    state->ApplyAction(legal_actions.at(i));
    EXPECT_FALSE(AllActionsInOtherActions(
        {legal_actions.at(i)}, state->PlayerCards(state->CurrentPlayer())));
  }
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
}

TEST(TarokStateTests, TestTalonExchangePhase4) {
  // 1 talon exchange, select the first set
  auto state = StateAfterActions(open_spiel::GameParameters(), {0, 0, 0, 4});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContract(), Contract::kOne);
  auto talon_initial = state->TalonSets();
  EXPECT_EQ(talon_initial.size(), 6);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 1, 2, 3, 4, 5));
  for (const auto& talon_set : talon_initial) {
    EXPECT_EQ(talon_set.size(), 1);
  }

  // select the first set
  state->ApplyAction(0);
  auto talon_end = state->TalonSets();
  EXPECT_EQ(talon_end.size(), 5);
  for (int i = 1; i < 6; i++) {
    EXPECT_EQ(talon_initial.at(i), talon_end.at(i - 1));
  }
  EXPECT_TRUE(AllActionsInOtherActions(
      talon_initial.at(0), state->PlayerCards(state->CurrentPlayer())));

  // discard the last card
  auto legal_actions = state->LegalActions();
  state->ApplyAction(legal_actions.at(legal_actions.size() - 1));
  EXPECT_FALSE(
      AllActionsInOtherActions({legal_actions.at(legal_actions.size() - 1)},
                               state->PlayerCards(state->CurrentPlayer())));
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
}

TEST(TarokStateTests, TestTalonExchangePhase5) {
  // 1 talon exchange, select the fourth set
  auto state = StateAfterActions(open_spiel::GameParameters(), {0, 0, 0, 4});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContract(), Contract::kOne);
  auto talon_initial = state->TalonSets();
  EXPECT_EQ(talon_initial.size(), 6);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 1, 2, 3, 4, 5));
  for (const auto& talon_set : talon_initial) {
    EXPECT_EQ(talon_set.size(), 1);
  }

  // select the fourth set
  state->ApplyAction(3);
  auto talon_end = state->TalonSets();
  EXPECT_EQ(talon_end.size(), 5);
  for (int i = 0; i < 5; i++) {
    if (i < 3)
      EXPECT_EQ(talon_initial.at(i), talon_end.at(i));
    else
      EXPECT_EQ(talon_initial.at(i + 1), talon_end.at(i));
  }
  EXPECT_TRUE(AllActionsInOtherActions(
      talon_initial.at(3), state->PlayerCards(state->CurrentPlayer())));

  // discard the second card
  auto legal_actions = state->LegalActions();
  state->ApplyAction(legal_actions.at(1));
  EXPECT_FALSE(AllActionsInOtherActions(
      {legal_actions.at(1)}, state->PlayerCards(state->CurrentPlayer())));
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
}

TEST(TarokStateTests, TestTalonExchangePhase6) {
  // 1 talon exchange, select the last set
  auto state = StateAfterActions(open_spiel::GameParameters(), {0, 0, 0, 4});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContract(), Contract::kOne);
  auto talon_initial = state->TalonSets();
  EXPECT_EQ(talon_initial.size(), 6);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 1, 2, 3, 4, 5));
  for (const auto& talon_set : talon_initial) {
    EXPECT_EQ(talon_set.size(), 1);
  }

  // select the last set
  state->ApplyAction(5);
  auto talon_end = state->TalonSets();
  EXPECT_EQ(talon_end.size(), 5);
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(talon_initial.at(i), talon_end.at(i));
  }
  EXPECT_TRUE(AllActionsInOtherActions(
      talon_initial.at(5), state->PlayerCards(state->CurrentPlayer())));

  // discard the first card
  auto legal_actions = state->LegalActions();
  state->ApplyAction(legal_actions.at(0));
  EXPECT_FALSE(AllActionsInOtherActions(
      {legal_actions.at(0)}, state->PlayerCards(state->CurrentPlayer())));
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
}

}  // namespace tarok
