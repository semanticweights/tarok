/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "test/tarok_utils.h"

namespace tarok {

static inline const open_spiel::GameParameters kGameParams =
    open_spiel::GameParameters({{"num_players", open_spiel::GameParameter(3)},
                                {"seed", open_spiel::GameParameter(634317)}});
// player 0 cards:
// ('II', 1), ('IIII', 3), ('V', 4), ('VIII', 7), ('XI', 10), ('XIX', 18),
// ('Mond', 20), ('Jack of Hearts', 26), ('Knight of Hearts', 27), ('4 of
// Diamonds', 30), ('8 of Spades', 39), ('Jack of Spades', 42), ('King of
// Spades', 45), ('10 of Clubs', 49), ('Jack of Clubs', 50), ('Knight of Clubs',
// 51)
//
// player 1 cards:
// ('III', 2), ('VII', 6), ('XII', 11), ('XIII', 12), ('XIV', 13), ('XX', 19),
// ('Skis', 21), ('1 of Hearts', 25), ('3 of Diamonds', 31), ('Knight of
// Diamonds', 35), ('Queen of Diamonds', 36), ('King of Diamonds', 37), ('7 of
// Spades', 38), ('Knight of Spades', 43), ('8 of Clubs', 47), ('Queen of
// Clubs', 52)
//
// player 2 cards:
// ('Pagat', 0), ('VI', 5), ('IX', 8), ('X', 9), ('XV', 14), ('XVI', 15),
// ('XVII', 16), ('XVIII', 17), ('4 of Hearts', 22), ('2 of Diamonds', 32), ('1
// of Diamonds', 33), ('Jack of Diamonds', 34), ('9 of Spades', 40), ('10 of
// Spades', 41), ('9 of Clubs', 48), ('King of Clubs', 53)

TEST(TarokStateTests, TestTricksPlayingPhase1) {
  // check forced pagat in klop
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 3, 4, 7, 10, 18, 20, 26, 27, 30, 39, 42,
                                   45, 49, 50, 51));
  state->ApplyAction(20);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(20));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(21));
  state->ApplyAction(21);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(20, 21));
  // pagat is forced
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(0));
  state->ApplyAction(0);
  // pagat won the trick
  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_TRUE(state->TrickCards().empty());
}

TEST(TarokStateTests, TestTricksPlayingPhase2) {
  // check pagat not a legal action in klop when following and all taroks lower
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 3, 4, 7, 10, 18, 20, 26, 27, 30, 39, 42,
                                   45, 49, 50, 51));
  state->ApplyAction(18);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(18));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(19, 21));
  state->ApplyAction(21);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(18, 21));
  // pagat not available but all other taroks available
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(5, 8, 9, 14, 15, 16, 17));
  state->ApplyAction(17);
  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_TRUE(state->TrickCards().empty());
}

TEST(TarokStateTests, TestTricksPlayingPhase3) {
  // check pagat not a legal action in klop when opening
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 3, 4, 7, 10, 18, 20, 26, 27, 30, 39, 42,
                                   45, 49, 50, 51));
  state->ApplyAction(4);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(4));
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(6, 11, 12, 13, 19, 21));
  state->ApplyAction(6);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(4, 6));
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(8, 9, 14, 15, 16, 17));
  state->ApplyAction(8);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(5, 9, 14, 15, 16, 17, 22, 32, 33, 34, 40, 41,
                                   48, 53));
}

TEST(TarokStateTests, TestTricksPlayingPhase4) {
  // check legal non-tarok cards in klop
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 3, 4, 7, 10, 18, 20, 26, 27, 30, 39, 42,
                                   45, 49, 50, 51));
  state->ApplyAction(42);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(42));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(43));
  state->ApplyAction(43);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(42, 43));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(40, 41));
  state->ApplyAction(41);
  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_TRUE(state->TrickCards().empty());
}

TEST(TarokStateTests, TestTricksPlayingPhase5) {
  // check scenarios where no card has to be beaten in klop
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 3, 4, 7, 10, 18, 20, 26, 27, 30, 39, 42,
                                   45, 49, 50, 51));
  state->ApplyAction(30);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(30));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(31, 35, 36, 37));
  state->ApplyAction(37);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(30, 37));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(32, 33, 34));
  state->ApplyAction(34);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(2, 6, 11, 12, 13, 19, 21, 25, 31, 35, 36, 38,
                                   43, 47, 52));
  state->ApplyAction(52);
  state->ApplyAction(53);
  state->ApplyAction(51);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(
      state->LegalActions(),
      testing::ElementsAre(5, 8, 9, 14, 15, 16, 17, 22, 32, 33, 40, 41, 48));
  state->ApplyAction(32);

  // can't follow suit, i.e. forced to play tarok
  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(32));
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 3, 4, 7, 10, 18, 20));
  state->ApplyAction(1);

  // doesn't have to beat the opening card due to the second card being tarok
  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(32, 1));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(31, 35, 36));
  state->ApplyAction(36);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_TRUE(state->TrickCards().empty());
}

TEST(TarokStateTests, TestTricksPlayingPhase6) {
  // check taroks don't win in colour valat
  auto state = StateAfterActions(kGameParams, {0, 11, 0, 0, 11});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kColourValatWithout);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(2, 6, 11, 12, 13, 19, 21, 25, 31, 35, 36, 37,
                                   38, 43, 47, 52));
  state->ApplyAction(35);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(35));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(32, 33, 34));
  state->ApplyAction(32);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(35, 32));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(30));
  state->ApplyAction(30);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(2, 6, 11, 12, 13, 19, 21, 25, 31, 36, 37, 38,
                                   43, 47, 52));
  state->ApplyAction(37);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(37));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(33, 34));
  state->ApplyAction(33);

  // can't follow suit, i.e. forced to play tarok
  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(37, 33));
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 3, 4, 7, 10, 18, 20));
  state->ApplyAction(1);

  // tarok didn't win the trick
  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_TRUE(state->TrickCards().empty());
}

TEST(TarokStateTests, TestTricksPlayingPhase7) {
  // check positive contracts scenarios
  auto state = StateAfterActions(kGameParams, {0, 0, 3, 0, 3, 0, 40, 41});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kTwo);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 3, 4, 7, 10, 18, 20, 26, 27, 30, 39, 42,
                                   45, 49, 50, 51));
  state->ApplyAction(30);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(30));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(31, 35, 36, 37));
  state->ApplyAction(31);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(30, 31));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(32, 33, 34));
  state->ApplyAction(32);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(0, 5, 8, 9, 14, 15, 16, 17, 22, 24, 28, 33,
                                   34, 48, 53));
  state->ApplyAction(33);

  // can't follow suit, i.e. forced to play tarok
  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(33));
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 3, 4, 7, 10, 18, 20));
  state->ApplyAction(18);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(33, 18));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(35, 36, 37));
  state->ApplyAction(37);

  // tarok won the trick
  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_TRUE(state->TrickCards().empty());
}

}  // namespace tarok
