/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "test/tarok_utils.h"

namespace tarok {

static inline const open_spiel::GameParameters kGameParams =
    open_spiel::GameParameters({{"num_players", open_spiel::GameParameter(3)},
                                {"seed", open_spiel::GameParameter(62240)}});
// player 0 cards:
// [('VIII', 7), ('XIII', 12), ('XVI', 15), ('XVII', 16), ('XX', 19), ('Mond',
// 20), ('4 of Hearts', 22), ('Queen of Hearts', 28), ('3 of Diamonds', 31), ('1
// of Diamonds', 33), ('King of Diamonds', 37), ('Knight of Spades', 43), ('King
// of Spades', 45), ('7 of Clubs', 46), ('8 of Clubs', 47), ('Knight of Clubs',
// 51)]
//
// player 1 cards:
// [('III', 2), ('V', 4), ('IX', 8), ('X', 9), ('XV', 14), ('XIX', 18), ('Skis',
// 21), ('2 of Hearts', 24), ('Jack of Hearts', 26), ('8 of Spades', 39), ('9 of
// Spades', 40), ('10 of Spades', 41), ('9 of Clubs', 48), ('10 of Clubs', 49),
// ('Jack of Clubs', 50), ('King of Clubs', 53)]
//
// player 2 cards:
// [('Pagat', 0), ('II', 1), ('VI', 5), ('VII', 6), ('XI', 10), ('XII', 11),
// ('XIV', 13), ('XVIII', 17), ('3 of Hearts', 23), ('1 of Hearts', 25),
// ('Knight of Hearts', 27), ('King of Hearts', 29), ('Knight of Diamonds', 35),
// ('7 of Spades', 38), ('Jack of Spades', 42), ('Queen of Clubs', 52)]

TEST(TarokStateTests, TestTricksPlayingPhase1) {
  // check forced pagat in klop
  auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
  EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
  EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

  EXPECT_EQ(state->CurrentPlayer(), 0);
  EXPECT_TRUE(state->TrickCards().empty());
  EXPECT_THAT(state->LegalActions(),
              testing::ElementsAre(7, 12, 15, 16, 19, 20, 22, 28, 31, 33, 37,
                                   43, 45, 46, 47, 51));
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

// TEST(TarokStateTests, TestTricksPlayingPhase2) {
//   // check pagat not a legal action in klop when following and all taroks
//   lower auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
//   EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
//   EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

//   EXPECT_EQ(state->CurrentPlayer(), 0);
//   EXPECT_TRUE(state->TrickCards().empty());
//   EXPECT_THAT(state->LegalActions(),
//               testing::ElementsAre(4, 6, 9, 11, 20, 25, 32, 33, 34, 36, 38,
//               40,
//                                    41, 43, 46, 53));
//   state->ApplyAction(11);

//   EXPECT_EQ(state->CurrentPlayer(), 1);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(11));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(12, 13, 15, 17,
//   21)); state->ApplyAction(21);

//   EXPECT_EQ(state->CurrentPlayer(), 2);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(11, 21));
//   // pagat not available but all other taroks available
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(3, 5, 7, 10, 19));
//   state->ApplyAction(19);
//   EXPECT_EQ(state->CurrentPlayer(), 1);
//   EXPECT_TRUE(state->TrickCards().empty());
// }

// TEST(TarokStateTests, TestTricksPlayingPhase3) {
//   // check pagat not a legal action in klop when opening
//   auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
//   EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
//   EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

//   EXPECT_EQ(state->CurrentPlayer(), 0);
//   EXPECT_TRUE(state->TrickCards().empty());
//   EXPECT_THAT(state->LegalActions(),
//               testing::ElementsAre(4, 6, 9, 11, 20, 25, 32, 33, 34, 36, 38,
//               40,
//                                    41, 43, 46, 53));
//   state->ApplyAction(4);

//   EXPECT_EQ(state->CurrentPlayer(), 1);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(4));
//   EXPECT_THAT(state->LegalActions(),
//               testing::ElementsAre(8, 12, 13, 15, 17, 21));
//   state->ApplyAction(8);

//   EXPECT_EQ(state->CurrentPlayer(), 2);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(4, 8));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(10, 19));
//   state->ApplyAction(10);

//   EXPECT_EQ(state->CurrentPlayer(), 2);
//   EXPECT_TRUE(state->TrickCards().empty());
//   EXPECT_THAT(state->LegalActions(),
//               testing::ElementsAre(3, 5, 7, 19, 23, 26, 31, 37, 39, 44, 47,
//               48,
//                                    49, 51));
// }

// TEST(TarokStateTests, TestTricksPlayingPhase4) {
//   // check only higher taroks legal in klop
//   auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
//   EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
//   EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

//   EXPECT_EQ(state->CurrentPlayer(), 0);
//   EXPECT_TRUE(state->TrickCards().empty());
//   EXPECT_THAT(state->LegalActions(),
//               testing::ElementsAre(4, 6, 9, 11, 20, 25, 32, 33, 34, 36, 38,
//               40,
//                                    41, 43, 46, 53));
//   state->ApplyAction(11);

//   EXPECT_EQ(state->CurrentPlayer(), 1);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(11));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(12, 13, 15, 17,
//   21)); state->ApplyAction(12);

//   EXPECT_EQ(state->CurrentPlayer(), 2);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(11, 12));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(19));
//   state->ApplyAction(19);
//   EXPECT_EQ(state->CurrentPlayer(), 2);
//   EXPECT_TRUE(state->TrickCards().empty());
// }

// TEST(TarokStateTests, TestTricksPlayingPhase5) {
//   // check legal non-tarok cards in klop
//   auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
//   EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
//   EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

//   EXPECT_EQ(state->CurrentPlayer(), 0);
//   EXPECT_TRUE(state->TrickCards().empty());
//   EXPECT_THAT(state->LegalActions(),
//               testing::ElementsAre(4, 6, 9, 11, 20, 25, 32, 33, 34, 36, 38,
//               40,
//                                    41, 43, 46, 53));
//   state->ApplyAction(36);

//   EXPECT_EQ(state->CurrentPlayer(), 1);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(36));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(30, 35));
//   state->ApplyAction(35);

//   EXPECT_EQ(state->CurrentPlayer(), 2);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(36, 35));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(37));
//   state->ApplyAction(37);
//   EXPECT_EQ(state->CurrentPlayer(), 2);
//   EXPECT_TRUE(state->TrickCards().empty());
// }

// TEST(TarokStateTests, TestTricksPlayingPhase6) {
//   // check scenarios where no card has to be beaten in klop
//   auto state = StateAfterActions(kGameParams, {0, 0, 0, 1});
//   EXPECT_EQ(state->CurrentGamePhase(), GamePhase::kTricksPlaying);
//   EXPECT_EQ(state->SelectedContract(), Contract::kKlop);

//   EXPECT_EQ(state->CurrentPlayer(), 0);
//   EXPECT_TRUE(state->TrickCards().empty());
//   EXPECT_THAT(state->LegalActions(),
//               testing::ElementsAre(4, 6, 9, 11, 20, 25, 32, 33, 34, 36, 38,
//               40,
//                                    41, 43, 46, 53));
//   state->ApplyAction(25);

//   EXPECT_EQ(state->CurrentPlayer(), 1);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(25));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(29));
//   state->ApplyAction(29);

//   EXPECT_EQ(state->CurrentPlayer(), 2);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(25, 29));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(23, 26));
//   state->ApplyAction(26);

//   EXPECT_EQ(state->CurrentPlayer(), 1);
//   EXPECT_TRUE(state->TrickCards().empty());
//   EXPECT_THAT(state->LegalActions(),
//               testing::ElementsAre(1, 2, 8, 12, 13, 15, 17, 21, 22, 24, 30,
//               35,
//                                    42, 50, 52));
//   state->ApplyAction(42);

//   EXPECT_EQ(state->CurrentPlayer(), 2);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(42));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(44));
//   state->ApplyAction(44);

//   EXPECT_EQ(state->CurrentPlayer(), 0);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(42, 44));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(38, 40, 41, 43));
//   state->ApplyAction(43);

//   EXPECT_EQ(state->CurrentPlayer(), 2);
//   EXPECT_TRUE(state->TrickCards().empty());
//   EXPECT_THAT(
//       state->LegalActions(),
//       testing::ElementsAre(3, 5, 7, 10, 19, 23, 31, 37, 39, 47, 48, 49, 51));
//   state->ApplyAction(23);

//   // can't follow suit, i.e. forced to play tarok
//   EXPECT_EQ(state->CurrentPlayer(), 0);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(23));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(4, 6, 9, 11, 20));
//   state->ApplyAction(4);

//   // doesn't have to beat the opening card due to the second card being tarok
//   EXPECT_EQ(state->CurrentPlayer(), 1);
//   EXPECT_THAT(state->TrickCards(), testing::ElementsAre(23, 4));
//   EXPECT_THAT(state->LegalActions(), testing::ElementsAre(22, 24));
//   state->ApplyAction(22);

//   EXPECT_EQ(state->CurrentPlayer(), 0);
//   EXPECT_TRUE(state->TrickCards().empty());
// }

}  // namespace tarok
