/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <algorithm>

#include "gtest/gtest.h"
#include "src/cards.h"
#include "test/tarok_utils.h"

namespace tarok {

class CardsTests : public ::testing::Test {
 protected:
  void SetUp() override {
    num_players_ = 3;
    std::tie(talon_, players_cards_) = DealCards(num_players_, 42);
  }

  int num_players_;
  std::vector<open_spiel::Action> talon_;
  std::vector<std::vector<open_spiel::Action>> players_cards_;
};

TEST_F(CardsTests, TestDealtCardsSize) {
  EXPECT_EQ(talon_.size(), 6);
  for (auto const& player_cards : players_cards_) {
    EXPECT_EQ(player_cards.size(), 16);
  }
}

TEST_F(CardsTests, TestDealtCardsContent) {
  // flatten and sort all the dealt cards
  std::vector<int> all_dealt_cards(talon_.begin(), talon_.end());
  for (auto const& player_cards : players_cards_) {
    all_dealt_cards.insert(all_dealt_cards.end(), player_cards.begin(),
                           player_cards.end());
  }
  std::sort(all_dealt_cards.begin(), all_dealt_cards.end());

  // check the actual content
  for (int i = 0; i < 54; i++) {
    EXPECT_EQ(all_dealt_cards.at(i), i);
  }
}

TEST_F(CardsTests, TestPlayersCardsSorted) {
  for (auto const& player_cards : players_cards_) {
    EXPECT_TRUE(std::is_sorted(player_cards.begin(), player_cards.end()));
  }
}

TEST_F(CardsTests, TestCountCards) {
  auto deck = tarok::InitializeCardDeck();
  std::vector<open_spiel::Action> all_card_actions(54);
  std::iota(all_card_actions.begin(), all_card_actions.end(), 0);
  EXPECT_EQ(tarok::CardPoints(all_card_actions, deck), 70);
  EXPECT_EQ(tarok::CardPoints({}, deck), 0);

  auto cards = {ActionFromCardName("XIV", deck)};
  EXPECT_EQ(tarok::CardPoints(cards, deck), 0);

  cards = {ActionFromCardName("Mond", deck)};
  EXPECT_EQ(tarok::CardPoints(cards, deck), 4);

  cards = {ActionFromCardName("Mond", deck),
           ActionFromCardName("Jack of Diamonds", deck)};
  EXPECT_EQ(tarok::CardPoints(cards, deck), 6);

  cards = {ActionFromCardName("XIV", deck), ActionFromCardName("Mond", deck),
           ActionFromCardName("Jack of Diamonds", deck)};
  EXPECT_EQ(tarok::CardPoints(cards, deck), 6);

  cards = {ActionFromCardName("XIV", deck), ActionFromCardName("Mond", deck),
           ActionFromCardName("Jack of Diamonds", deck),
           ActionFromCardName("Queen of Diamonds", deck)};
  EXPECT_EQ(tarok::CardPoints(cards, deck), 9);

  cards = {ActionFromCardName("XIV", deck), ActionFromCardName("Mond", deck),
           ActionFromCardName("Jack of Diamonds", deck),
           ActionFromCardName("Queen of Diamonds", deck),
           ActionFromCardName("King of Clubs", deck)};
  EXPECT_EQ(tarok::CardPoints(cards, deck), 14);
}

}  // namespace tarok
