/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "test/state_tests.h"
#include "test/tarok_utils.h"

namespace tarok {

TEST_F(TarokStateTests, TestTalonExchangePhase1) {
  // 3 talon exchanges, select the first set
  auto state = StateAfterActions(
      open_spiel::GameParameters(),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidThreeAction});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kThree);
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

TEST_F(TarokStateTests, TestTalonExchangePhase2) {
  // 3 talon exchanges, select the second set
  auto state = StateAfterActions(
      open_spiel::GameParameters(),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidThreeAction});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kThree);
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

TEST_F(TarokStateTests, TestTalonExchangePhase3) {
  // 2 talon exchanges, select the middle set
  auto state = StateAfterActions(
      open_spiel::GameParameters(),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidTwoAction});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kTwo);
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

TEST_F(TarokStateTests, TestTalonExchangePhase4) {
  // 1 talon exchange, select the first set
  auto state = StateAfterActions(
      open_spiel::GameParameters(),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidOneAction});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOne);
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

TEST_F(TarokStateTests, TestTalonExchangePhase5) {
  // 1 talon exchange, select the fourth set
  auto state = StateAfterActions(
      open_spiel::GameParameters(),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidOneAction});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOne);
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

TEST_F(TarokStateTests, TestTalonExchangePhase6) {
  // 1 talon exchange, select the last set
  auto state = StateAfterActions(
      open_spiel::GameParameters(),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidOneAction});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOne);
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

TEST_F(TarokStateTests, TestTalonExchangePhase7) {
  // check that taroks and kings cannot be exchanged
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"seed", open_spiel::GameParameter(42)}}),
      {kDealCardsAction, kBidPassAction, kBidOneAction, kBidPassAction,
       kBidOneAction, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kOne);

  // check taroks and kings are not in legal actions
  for (const auto& action : state->LegalActions()) {
    auto card = deck_.at(action);
    EXPECT_NE(card.suit, CardSuit::kTaroks);
    EXPECT_NE(card.points, 5);
  }
}

TEST_F(TarokStateTests, TestTalonExchangePhase8) {
  // check that tarok can be exchanged if player has no other choice
  auto state = StateAfterActions(
      open_spiel::GameParameters({{"num_players", open_spiel::GameParameter(4)},
                                  {"seed", open_spiel::GameParameter(141750)}}),
      {kDealCardsAction, kBidPassAction, kBidPassAction, kBidPassAction,
       kBidSoloTwoAction});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);
  EXPECT_EQ(state->SelectedContractName(), ContractName::kSoloTwo);

  // select first set from talon
  state->ApplyAction(0);
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);

  // first the player must exchange non-tarok or non-king card
  // check taroks and kings are not in legal actions
  for (const auto& action : state->LegalActions()) {
    auto card = deck_.at(action);
    EXPECT_NE(card.suit, CardSuit::kTaroks);
    EXPECT_NE(card.points, 5);
  }
  state->ApplyAction(state->LegalActions().at(0));
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTalonExchange);

  // at this point the player has only taroks and kings in his hand but still
  // needs to exchange one card
  // check only taroks (no trula or kings) are in legal actions
  for (const auto& action : state->LegalActions()) {
    auto card = deck_.at(action);
    EXPECT_EQ(card.suit, CardSuit::kTaroks);
    EXPECT_NE(card.points, 5);
  }
  state->ApplyAction(state->LegalActions().at(0));
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
}
}  // namespace tarok
