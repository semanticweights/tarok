/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "test/tarok_utils.h"

namespace tarok {

TEST(TarokStateTests, TestCapturedMond1) {
  // mond captured by skis
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(634317)}}),
      {0, 0, 0, 4, 0, 49});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kOne);

  // play mond
  EXPECT_EQ(state->CurrentPlayer(), 0);
  state->ApplyAction(20);
  // play skis
  EXPECT_EQ(state->CurrentPlayer(), 1);
  state->ApplyAction(21);
  // play low tarok
  EXPECT_EQ(state->CurrentPlayer(), 2);
  state->ApplyAction(5);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->CapturedMondPenalties(),
              testing::ElementsAre(-20.0, 0.0, 0.0));
}

TEST(TarokStateTests, TestCapturedMond2) {
  // mond captured by pagat (emperor trick)
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(634317)}}),
      {0, 0, 0, 4, 0, 49});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kOne);

  // play mond
  EXPECT_EQ(state->CurrentPlayer(), 0);
  state->ApplyAction(20);
  // play skis
  EXPECT_EQ(state->CurrentPlayer(), 1);
  state->ApplyAction(21);
  // play pagat
  EXPECT_EQ(state->CurrentPlayer(), 2);
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->CapturedMondPenalties(),
              testing::ElementsAre(-20.0, 0.0, 0.0));
}

TEST(TarokStateTests, TestCapturedMond3) {
  // mond taken from talon
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(497200)}}),
      {0, 0, 0, 4, 3, 49});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kOne);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->CapturedMondPenalties(),
              testing::ElementsAre(0.0, 0.0, 0.0));
}

TEST(TarokStateTests, TestCapturedMond4) {
  // mond left in talon
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(497200)}}),
      {0, 0, 0, 4, 0, 49});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kOne);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->CapturedMondPenalties(),
              testing::ElementsAre(-20.0, 0.0, 0.0));
}

TEST(TarokStateTests, TestCapturedMond5) {
  // mond left in talon but won with a called king
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"num_players", open_spiel::GameParameter(4)},
                                  {"seed", open_spiel::GameParameter(297029)}}),
      {0, 0, 0, 0, 4, 45, 2, 49});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kOne);

  // play the called king and win the trick
  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->CapturedMondPenalties(),
              testing::ElementsAre(-20.0, 0.0, 0.0, 0.0));
  state->ApplyAction(45);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  state->ApplyAction(44);
  EXPECT_EQ(state->CurrentPlayer(), 2);
  state->ApplyAction(39);
  EXPECT_EQ(state->CurrentPlayer(), 3);
  state->ApplyAction(38);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->CapturedMondPenalties(),
              testing::ElementsAre(0.0, 0.0, 0.0, 0.0));
}

}  // namespace tarok
