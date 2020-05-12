/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "gtest/gtest.h"
#include "src/cards.h"

namespace tarok {

TEST(CardsTests, TestDealCards) {
  auto [talon, private_cards] = DealCards(3, 42);

  EXPECT_EQ(talon.size(), 6);  // first element is talon and has 6 cards
  EXPECT_EQ(private_cards[0].size(), 16);  // each player has 16 cards
  EXPECT_EQ(private_cards[1].size(), 16);  // each player has 16 cards
  EXPECT_EQ(private_cards[2].size(), 16);  // each player has 16 cards

  std::array<std::vector<int>, 4> splits = {talon, private_cards[0],
                                            private_cards[1], private_cards[2]};

  for (int i = 0; i < splits.size() - 1; i++) {
    // sort cards
    sort(splits[i].begin(), splits[i].end());
    // lowest card must be greater or equal to 0
    EXPECT_GE(splits[i][0], 0);
    // highest card must be lower or equal to 53
    EXPECT_LE(splits[i][0], 53);

    for (int j = i + 1; j < splits.size(); j++) {
      // sort cards
      sort(splits[j].begin(), splits[j].end());
      // lowest card must be greater or equal to 0
      EXPECT_GE(splits[j][0], 0);
      // highest card must be lower or equal to 53
      EXPECT_LE(splits[j][0], 53);

      // find intersection
      std::vector<int> intersection;
      std::set_intersection(splits[i].begin(), splits[i].end(),
                            splits[j].begin(), splits[j].end(),
                            back_inserter(intersection));
      // intersection must be an empty set
      EXPECT_EQ(intersection.size(), 0);
    }
  }
}

}  // namespace tarok
