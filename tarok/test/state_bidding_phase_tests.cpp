/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/game.h"
#include "test/state_tests.h"

namespace tarok {

// 3 players
TEST_F(TarokStateTests, TestBiddingPhase3Players1) {
  // scenario: all players pass
  auto game = NewTarokGame(open_spiel::GameParameters());
  auto state = game->NewInitialTarokState();
  state->ApplyAction(kDealCardsAction);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidTwoAction, kBidOneAction,
                                   kBidBeggarAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidTwoAction, kBidOneAction,
                                   kBidBeggarAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidKlopAction, kBidThreeAction, kBidTwoAction, kBidOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidKlopAction);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kKlop);
  EXPECT_EQ(state->CurrentPlayer(), 0);
}

TEST_F(TarokStateTests, TestBiddingPhase3Players2) {
  // scenario: forehand passes, player 1 eventually
  // bids beggar, player 2 bids beggar
  auto game = NewTarokGame(open_spiel::GameParameters());
  auto state = game->NewInitialTarokState();
  state->ApplyAction(kDealCardsAction);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidTwoAction, kBidOneAction,
                                   kBidBeggarAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidTwoAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(
      state->LegalActions(),
      testing::ElementsAre(kBidPassAction, kBidOneAction, kBidBeggarAction,
                           kSoloWithoutAction, kBidOpenBeggarAction,
                           kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidBeggarAction);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(
      state->LegalActions(),
      testing::ElementsAre(kBidPassAction, kBidBeggarAction, kSoloWithoutAction,
                           kBidOpenBeggarAction, kBidColourValatAction,
                           kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(
      state->LegalActions(),
      testing::ElementsAre(kBidPassAction, kBidBeggarAction, kSoloWithoutAction,
                           kBidOpenBeggarAction, kBidColourValatAction,
                           kBidValatWithoutAction));
  state->ApplyAction(kBidBeggarAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidBeggarAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidBeggarAction);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kBeggar);
  EXPECT_EQ(state->CurrentPlayer(), 1);
}

TEST_F(TarokStateTests, TestBiddingPhase3Players3) {
  // scenario: forehand passes, player 1 bids beggar, player 2 bids solo without
  auto game = NewTarokGame(open_spiel::GameParameters());
  auto state = game->NewInitialTarokState();
  state->ApplyAction(kDealCardsAction);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidTwoAction, kBidOneAction,
                                   kBidBeggarAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidBeggarAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kSoloWithoutAction);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(
      state->LegalActions(),
      testing::ElementsAre(kSoloWithoutAction, kBidOpenBeggarAction,
                           kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kSoloWithoutAction);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kSoloWithout);
  EXPECT_EQ(state->CurrentPlayer(), 2);
}

TEST_F(TarokStateTests, TestBiddingPhase3Players4) {
  // scenario: forehand bids valat without, others are forced to pass
  // todo: we could check this case in DoApplyActionInBidding
  // and simply finish the bidding phase early
  auto game = NewTarokGame(open_spiel::GameParameters());
  auto state = game->NewInitialTarokState();
  state->ApplyAction(kDealCardsAction);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidTwoAction, kBidOneAction,
                                   kBidBeggarAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidTwoAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(
      state->LegalActions(),
      testing::ElementsAre(kBidPassAction, kBidOneAction, kBidBeggarAction,
                           kSoloWithoutAction, kBidOpenBeggarAction,
                           kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidOneAction);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(
      state->LegalActions(),
      testing::ElementsAre(kBidPassAction, kBidOneAction, kBidBeggarAction,
                           kSoloWithoutAction, kBidOpenBeggarAction,
                           kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidValatWithoutAction);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(kBidPassAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(kBidPassAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidValatWithoutAction));
  state->ApplyAction(kBidValatWithoutAction);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kValatWithout);
  EXPECT_EQ(state->CurrentPlayer(), 0);
}

// 4 players
TEST_F(TarokStateTests, TestBiddingPhase4Players1) {
  // scenario: all players pass
  auto game = NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(4)}}));
  auto state = game->NewInitialTarokState();
  state->ApplyAction(kDealCardsAction);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidTwoAction, kBidOneAction,
                  kBidSoloThreeAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidTwoAction, kBidOneAction,
                  kBidSoloThreeAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidTwoAction, kBidOneAction,
                  kBidSoloThreeAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidKlopAction, kBidThreeAction, kBidTwoAction, kBidOneAction,
                  kBidSoloThreeAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidKlopAction);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kKlop);
  EXPECT_EQ(state->CurrentPlayer(), 0);
}

TEST_F(TarokStateTests, TestBiddingPhase4Players2) {
  // scenario: forehand bids one, player 2 bids one, others pass
  auto game = NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(4)}}));
  auto state = game->NewInitialTarokState();
  state->ApplyAction(kDealCardsAction);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidTwoAction, kBidOneAction,
                  kBidSoloThreeAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidTwoAction, kBidOneAction,
                  kBidSoloThreeAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidOneAction);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidSoloThreeAction,
                                   kBidSoloTwoAction, kBidSoloOneAction,
                                   kBidBeggarAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(
      state->LegalActions(),
      testing::ElementsAre(
          kBidPassAction, kBidOneAction, kBidSoloThreeAction, kBidSoloTwoAction,
          kBidSoloOneAction, kBidBeggarAction, kSoloWithoutAction,
          kBidOpenBeggarAction, kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidOneAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidSoloThreeAction,
                                   kBidSoloTwoAction, kBidSoloOneAction,
                                   kBidBeggarAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidOneAction, kBidSoloThreeAction,
                                   kBidSoloTwoAction, kBidSoloOneAction,
                                   kBidBeggarAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidOneAction);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kKingCalling);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOne);
  EXPECT_EQ(state->CurrentPlayer(), 0);
}

TEST_F(TarokStateTests, TestBiddingPhase4Players3) {
  // scenario: player 1 bids solo three, player 3 eventually bids solo one,
  // others pass
  auto game = NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(4)}}));
  auto state = game->NewInitialTarokState();
  state->ApplyAction(kDealCardsAction);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidTwoAction, kBidOneAction,
                  kBidSoloThreeAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidSoloThreeAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidSoloTwoAction);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidSoloTwoAction, kBidSoloOneAction,
                                   kBidBeggarAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidSoloOneAction);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kSoloOne);
  EXPECT_EQ(state->CurrentPlayer(), 3);
}

TEST_F(TarokStateTests, TestBiddingPhase4Players4) {
  // scenario: player 2 bids beggar, others pass
  auto game = NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(4)}}));
  auto state = game->NewInitialTarokState();
  state->ApplyAction(kDealCardsAction);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidTwoAction, kBidOneAction,
                  kBidSoloThreeAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidTwoAction, kBidOneAction,
                  kBidSoloThreeAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidBeggarAction);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(
      state->LegalActions(),
      testing::ElementsAre(kBidPassAction, kBidBeggarAction, kSoloWithoutAction,
                           kBidOpenBeggarAction, kBidColourValatAction,
                           kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidBeggarAction, kSoloWithoutAction,
                                   kBidOpenBeggarAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidBeggarAction);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kBeggar);
  EXPECT_EQ(state->CurrentPlayer(), 2);
}

TEST_F(TarokStateTests, TestBiddingPhase4Players5) {
  // scenario: forehand passes, player 1 bids open beggar, player 2 bids
  // colour valat without, player 3 bids valat without
  auto game = NewTarokGame(open_spiel::GameParameters(
      {{"num_players", open_spiel::GameParameter(4)}}));
  auto state = game->NewInitialTarokState();
  state->ApplyAction(kDealCardsAction);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kBidding);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kNotSelected);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(
                  kBidPassAction, kBidTwoAction, kBidOneAction,
                  kBidSoloThreeAction, kBidSoloTwoAction, kBidSoloOneAction,
                  kBidBeggarAction, kSoloWithoutAction, kBidOpenBeggarAction,
                  kBidColourValatAction, kBidValatWithoutAction));
  state->ApplyAction(kBidOpenBeggarAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidColourValatAction,
                                   kBidValatWithoutAction));
  state->ApplyAction(kBidColourValatAction);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidValatWithoutAction));
  state->ApplyAction(kBidValatWithoutAction);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidPassAction, kBidValatWithoutAction));
  state->ApplyAction(kBidPassAction);

  EXPECT_EQ(state->CurrentPlayer(), 3);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(kBidValatWithoutAction));
  state->ApplyAction(kBidValatWithoutAction);

  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kValatWithout);
  EXPECT_EQ(state->CurrentPlayer(), 3);
}

}  // namespace tarok
