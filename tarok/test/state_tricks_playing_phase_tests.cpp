/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "test/tarok_utils.h"

namespace tarok {

static inline const open_spiel::GameParameters kGameParams =
    open_spiel::GameParameters({{"num_players", open_spiel::GameParameter(3)},
                                {"seed", open_spiel::GameParameter(544216)}});
// player 0 cards:
// [('V', 4), ('VII', 6), ('X', 9), ('XII', 11), ('Mond', 20), ('1 of Hearts',
// 25), ('2 of Diamonds', 32), ('1 of Diamonds', 33), ('Jack of Diamonds', 34),
// ('Queen of Diamonds', 36), ('7 of Spades', 38), ('9 of Spades', 40), ('10 of
// Spades', 41), ('Knight of Spades', 43), ('7 of Clubs', 46), ('King of Clubs',
// 53)]
//
// player 1 cards:
// [('II', 1), ('III', 2), ('IX', 8), ('XIII', 12), ('XIV', 13), ('XVI', 15),
// ('XVIII', 17), ('Skis', 21), ('4 of Hearts', 22), ('2 of Hearts', 24), ('King
// of Hearts', 29), ('4 of Diamonds', 30), ('Knight of Diamonds', 35), ('Jack of
// Spades', 42), ('Jack of Clubs', 50), ('Queen of Clubs', 52)]
//
// player 2 cards:
// [('Pagat', 0), ('IIII', 3), ('VI', 5), ('VIII', 7), ('XI', 10), ('XX', 19),
// ('3 of Hearts', 23), ('Jack of Hearts', 26), ('3 of Diamonds', 31), ('King of
// Diamonds', 37), ('8 of Spades', 39), ('Queen of Spades', 44), ('8 of Clubs',
// 47), ('9 of Clubs', 48), ('10 of Clubs', 49), ('Knight of Clubs', 51)]

TEST(TarokStateTests, TestTricksPlayingPhase1) {
  // check forced pagat in klop
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_EQ(state->TrickCards().size(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(4, 6, 9, 11, 20, 25, 32, 33, 34, 36, 38, 40,
                                   41, 43, 46, 53));
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
  EXPECT_EQ(state->TrickCards().size(), 0);
}

TEST(TarokStateTests, TestTricksPlayingPhase2) {
  // check pagat not a legal action in klop when following and all taroks lower
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_EQ(state->TrickCards().size(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(4, 6, 9, 11, 20, 25, 32, 33, 34, 36, 38, 40,
                                   41, 43, 46, 53));
  state->ApplyAction(11);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(11));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(12, 13, 15, 17, 21));
  state->ApplyAction(21);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(11, 21));
  // pagat not available but all other taroks available
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(3, 5, 7, 10, 19));
  state->ApplyAction(19);
  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_EQ(state->TrickCards().size(), 0);
}

TEST(TarokStateTests, TestTricksPlayingPhase3) {
  // check pagat not a legal action in klop when opening
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_EQ(state->TrickCards().size(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(4, 6, 9, 11, 20, 25, 32, 33, 34, 36, 38, 40,
                                   41, 43, 46, 53));
  state->ApplyAction(4);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(4));
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(8, 12, 13, 15, 17, 21));
  state->ApplyAction(8);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(4, 8));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(10, 19));
  state->ApplyAction(10);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_EQ(state->TrickCards().size(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(3, 5, 7, 19, 23, 26, 31, 37, 39, 44, 47, 48,
                                   49, 51));
}

TEST(TarokStateTests, TestTricksPlayingPhase4) {
  // check only higher taroks legal in klop
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_EQ(state->TrickCards().size(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(4, 6, 9, 11, 20, 25, 32, 33, 34, 36, 38, 40,
                                   41, 43, 46, 53));
  state->ApplyAction(11);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(11));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(12, 13, 15, 17, 21));
  state->ApplyAction(12);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(11, 12));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(19));
  state->ApplyAction(19);
  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_EQ(state->TrickCards().size(), 0);
}

TEST(TarokStateTests, TestTricksPlayingPhase5) {
  // check legal non-tarok cards in klop
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_EQ(state->TrickCards().size(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(4, 6, 9, 11, 20, 25, 32, 33, 34, 36, 38, 40,
                                   41, 43, 46, 53));
  state->ApplyAction(36);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(36));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(30, 35));
  state->ApplyAction(35);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(36, 35));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(37));
  state->ApplyAction(37);
  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_EQ(state->TrickCards().size(), 0);
}

TEST(TarokStateTests, TestTricksPlayingPhase6) {
  // check scenarios where no card has to be beaten in klop
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_EQ(state->TrickCards().size(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(4, 6, 9, 11, 20, 25, 32, 33, 34, 36, 38, 40,
                                   41, 43, 46, 53));
  state->ApplyAction(25);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(25));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(29));
  state->ApplyAction(29);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(25, 29));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(23, 26));
  state->ApplyAction(26);

  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_EQ(state->TrickCards().size(), 0);
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(1, 2, 8, 12, 13, 15, 17, 21, 22, 24, 30, 35,
                                   42, 50, 52));
  state->ApplyAction(42);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(42));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(44));
  state->ApplyAction(44);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(42, 44));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(38, 40, 41, 43));
  state->ApplyAction(43);

  EXPECT_EQ(state->CurrentPlayer(), 2);
  EXPECT_EQ(state->TrickCards().size(), 0);
  EXPECT_THAT(
      state->LegalActions(),
      testing::ElementsAre(3, 5, 7, 10, 19, 23, 31, 37, 39, 47, 48, 49, 51));
  state->ApplyAction(23);

  // can't follow suit, i.e. forced to play tarok
  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(23));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(4, 6, 9, 11, 20));
  state->ApplyAction(4);

  // doesn't have to beat the opening card due to the second card being tarok
  EXPECT_EQ(state->CurrentPlayer(), 1);
  EXPECT_THAT(state->TrickCards(), testing::ElementsAre(23, 4));
  EXPECT_THAT(state->LegalActions(), testing::ElementsAre(22, 24));
  state->ApplyAction(22);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_EQ(state->TrickCards().size(), 0);
}

}  // namespace tarok
