/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "open_spiel/spiel.h"
#include "src/contracts.h"
#include "src/game.h"
#include "src/state.h"

namespace tarok {

// 3 players
TEST(TarokStateTests, TestBiddingPhase3Players1) {
  // scenario: all players pass
  auto game = NewTarokGame(open_spiel::GameParameters());
  auto state = game->NewInitialTarokState();
  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContract(), Contract::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 2, 3, 4, 8, 9, 10, 11, 12));
  state->ApplyAction(1);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);
  EXPECT_EQ(state->CurrentPlayer(), 0);
}

TEST(TarokStateTests, TestBiddingPhase3Players2) {
  // scenario: forehand passes, player 1 eventually
  // bids beggar, player 2 bids beggar
  auto game = NewTarokGame(open_spiel::GameParameters());
  auto state = game->NewInitialTarokState();
  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContract(), Contract::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 8, 9, 10, 11, 12));
  state->ApplyAction(3);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 4, 8, 9, 10, 11, 12));
  state->ApplyAction(8);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 8, 9, 10, 11, 12));
  state->ApplyAction(8);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(8, 9, 10, 11, 12));
  state->ApplyAction(8);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kBeggar);
  EXPECT_EQ(state->CurrentPlayer(), 1);
}

TEST(TarokStateTests, TestBiddingPhase3Players3) {
  // scenario: forehand passes, player 1 bids beggar, player 2 bids solo without
  auto game = NewTarokGame(open_spiel::GameParameters());
  auto state = game->NewInitialTarokState();
  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContract(), Contract::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 8, 9, 10, 11, 12));
  state->ApplyAction(8);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 9, 10, 11, 12));
  state->ApplyAction(9);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(9, 10, 11, 12));
  state->ApplyAction(9);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kSoloWithout);
  EXPECT_EQ(state->CurrentPlayer(), 2);
}

TEST(TarokStateTests, TestBiddingPhase3Players4) {
  // scenario: forehand bids valat without, others are forced to pass
  // todo: we could check this case in DoApplyActionInBidding
  // and simply finish the bidding phase early
  auto game = NewTarokGame(open_spiel::GameParameters());
  auto state = game->NewInitialTarokState();
  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContract(), Contract::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 8, 9, 10, 11, 12));
  state->ApplyAction(3);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 4, 8, 9, 10, 11, 12));
  state->ApplyAction(4);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 4, 8, 9, 10, 11, 12));
  state->ApplyAction(12);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(12));
  state->ApplyAction(12);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kValatWithout);
  EXPECT_EQ(state->CurrentPlayer(), 0);
}

// 4 players
TEST(TarokStateTests, TestBiddingPhase4Players1) {
  // scenario: all players pass
  auto game = NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(4)}}));
  auto state = game->NewInitialTarokState();
  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContract(), Contract::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(1);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);
  EXPECT_EQ(state->CurrentPlayer(), 0);
}

TEST(TarokStateTests, TestBiddingPhase4Players2) {
  // scenario: forehand bids one, player 2 bids one, others pass
  auto game = NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(4)}}));
  auto state = game->NewInitialTarokState();
  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContract(), Contract::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(4);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(4);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(4);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kKingCalling);
  EXPECT_EQ(state->SelectedContract(), Contract::kOne);
  EXPECT_EQ(state->CurrentPlayer(), 0);
}

TEST(TarokStateTests, TestBiddingPhase4Players3) {
  // scenario: player 1 bids solo three, player 3 eventually bids solo one,
  // others pass
  auto game = NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(4)}}));
  auto state = game->NewInitialTarokState();
  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContract(), Contract::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(5);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(6);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(7);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContract(), Contract::kSoloOne);
  EXPECT_EQ(state->CurrentPlayer(), 3);
}

TEST(TarokStateTests, TestBiddingPhase4Players4) {
  // scenario: player 2 bids beggar, others pass
  auto game = NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(4)}}));
  auto state = game->NewInitialTarokState();
  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContract(), Contract::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(8);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 8, 9, 10, 11, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(8, 9, 10, 11, 12));
  state->ApplyAction(8);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kBeggar);
  EXPECT_EQ(state->CurrentPlayer(), 2);
}

TEST(TarokStateTests, TestBiddingPhase4Players5) {
  // scenario: forehand passes, player 1 bids open beggar, player 2 bids
  // colour valat without, player 3 bids valat without
  auto game = NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(4)}}));
  auto state = game->NewInitialTarokState();
  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContract(), Contract::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
  state->ApplyAction(10);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 11, 12));
  state->ApplyAction(11);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 12));
  state->ApplyAction(12);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0, 12));
  state->ApplyAction(0);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(12));
  state->ApplyAction(12);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kValatWithout);
  EXPECT_EQ(state->CurrentPlayer(), 3);
}

}  // namespace tarok
