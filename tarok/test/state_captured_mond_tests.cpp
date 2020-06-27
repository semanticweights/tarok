/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/cards.h"
#include "test/state_tests.h"
#include "test/tarok_utils.h"

namespace tarok {

TEST_F(TarokStateTests, TestCapturedMond1) {
  // mond captured by skis
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(634317)}}),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidOneAction, 0, 49});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOne);

  // play mond
  EXPECT_EQ(state->CurrentPlayer(), 0);
  state->ApplyAction(CardLongNameToAction("Mond", deck_));
  // play skis
  EXPECT_EQ(state->CurrentPlayer(), 1);
  state->ApplyAction(CardLongNameToAction("Skis", deck_));
  // play low tarok
  EXPECT_EQ(state->CurrentPlayer(), 2);
  state->ApplyAction(CardLongNameToAction("VI", deck_));

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->CapturedMondPenalties(), testing::ElementsAre(-20, 0, 0));
}

TEST_F(TarokStateTests, TestCapturedMond2) {
  // mond captured by pagat (emperor trick)
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(634317)}}),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidOneAction, 0, 49});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOne);

  // play mond
  EXPECT_EQ(state->CurrentPlayer(), 0);
  state->ApplyAction(CardLongNameToAction("Mond", deck_));
  // play skis
  EXPECT_EQ(state->CurrentPlayer(), 1);
  state->ApplyAction(CardLongNameToAction("Skis", deck_));
  // play pagat
  EXPECT_EQ(state->CurrentPlayer(), 2);
  state->ApplyAction(CardLongNameToAction("Pagat", deck_));

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->CapturedMondPenalties(), testing::ElementsAre(-20, 0, 0));
}

TEST_F(TarokStateTests, TestCapturedMond3) {
  // mond taken from talon
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(497200)}}),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidOneAction, 3, 49});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOne);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->CapturedMondPenalties(), testing::ElementsAre(0, 0, 0));
}

TEST_F(TarokStateTests, TestCapturedMond4) {
  // mond left in talon
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(497200)}}),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidOneAction, 0, 49});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOne);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->CapturedMondPenalties(), testing::ElementsAre(-20, 0, 0));
}

TEST_F(TarokStateTests, TestCapturedMond5) {
  // mond left in talon but won with a called king
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"num_players", open_spiel::GameParameter(4)},
                                  {"seed", open_spiel::GameParameter(297029)}}),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidPassAction,
       kBidOneAction, kKingOfSpadesAction, 2, 49});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOne);

  // play the called king and win the trick
  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->CapturedMondPenalties(),
              testing::ElementsAre(-20, 0, 0, 0));
  state->ApplyAction(CardLongNameToAction("King of Spades", deck_));

  EXPECT_EQ(state->CurrentPlayer(), 1);
  state->ApplyAction(CardLongNameToAction("Queen of Spades", deck_));
  EXPECT_EQ(state->CurrentPlayer(), 2);
  state->ApplyAction(CardLongNameToAction("8 of Spades", deck_));
  EXPECT_EQ(state->CurrentPlayer(), 3);
  state->ApplyAction(CardLongNameToAction("7 of Spades", deck_));

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->CapturedMondPenalties(), testing::ElementsAre(0, 0, 0, 0));
}

TEST_F(TarokStateTests, TestCapturedMond6) {
  // mond captured by ally should also be penalized
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(634317)}}),
      {kDealCardsAction, kBidPassAction, kBidOneAction, kBidPassAction,
       kBidOneAction, 0, 22});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOne);

  // play mond
  EXPECT_EQ(state->CurrentPlayer(), 0);
  state->ApplyAction(CardLongNameToAction("Mond", deck_));
  // play skis
  EXPECT_EQ(state->CurrentPlayer(), 1);
  state->ApplyAction(CardLongNameToAction("Skis", deck_));
  // play low tarok
  EXPECT_EQ(state->CurrentPlayer(), 2);
  state->ApplyAction(CardLongNameToAction("VI", deck_));

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->CapturedMondPenalties(), testing::ElementsAre(-20, 0, 0));
}

TEST_F(TarokStateTests, TestCapturedMond7) {
  // mond captured in klop should not be penalized
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(634317)}}),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidKlopAction});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kKlop);

  // play mond
  EXPECT_EQ(state->CurrentPlayer(), 0);
  state->ApplyAction(CardLongNameToAction("Mond", deck_));
  // play skis
  EXPECT_EQ(state->CurrentPlayer(), 1);
  state->ApplyAction(CardLongNameToAction("Skis", deck_));
  // play pagat
  EXPECT_EQ(state->CurrentPlayer(), 2);
  state->ApplyAction(CardLongNameToAction("Pagat", deck_));

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->CapturedMondPenalties(), testing::ElementsAre(0, 0, 0));
}

TEST_F(TarokStateTests, TestCapturedMond8) {
  // mond captured in bagger should not be penalized
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(634317)}}),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidBeggarAction});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kBeggar);

  // play mond
  EXPECT_EQ(state->CurrentPlayer(), 0);
  state->ApplyAction(CardLongNameToAction("Mond", deck_));
  // play skis
  EXPECT_EQ(state->CurrentPlayer(), 1);
  state->ApplyAction(CardLongNameToAction("Skis", deck_));
  // play pagat
  EXPECT_EQ(state->CurrentPlayer(), 2);
  state->ApplyAction(CardLongNameToAction("Pagat", deck_));

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->CapturedMondPenalties(), testing::ElementsAre(0, 0, 0));
}

TEST_F(TarokStateTests, TestCapturedMond9) {
  // mond captured in open bagger should not be penalized
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(634317)}}),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidOpenBeggarAction});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOpenBeggar);

  // play mond
  EXPECT_EQ(state->CurrentPlayer(), 0);
  state->ApplyAction(CardLongNameToAction("Mond", deck_));
  // play skis
  EXPECT_EQ(state->CurrentPlayer(), 1);
  state->ApplyAction(CardLongNameToAction("Skis", deck_));
  // play pagat
  EXPECT_EQ(state->CurrentPlayer(), 2);
  state->ApplyAction(CardLongNameToAction("Pagat", deck_));

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->CapturedMondPenalties(), testing::ElementsAre(0, 0, 0));
}

}  // namespace tarok
