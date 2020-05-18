/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <algorithm>

#include "gtest/gtest.h"
#include "src/cards.h"

namespace tarok {

class TarokCardsTests : public ::testing::Test {
 protected:
  void SetUp() override {
    num_players_ = 3;
    std::tie(talon_, players_cards_) = DealCards(num_players_, 42);
  }

  int num_players_;
  std::vector<int> talon_;
  std::vector<std::vector<int>> players_cards_;
};

TEST_F(TarokCardsTests, TestDealtCardsSize) {
  EXPECT_EQ(talon_.size(), 6);
  for (auto const& player_cards : players_cards_) {
    EXPECT_EQ(player_cards.size(), 16);
  }
}

TEST_F(TarokCardsTests, TestDealtCardsContent) {
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

TEST_F(TarokCardsTests, TestPlayersCardsSorted) {
  for (auto const& player_cards : players_cards_) {
    EXPECT_TRUE(std::is_sorted(player_cards.begin(), player_cards.end()));
  }
}

}  // namespace tarok
