/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include "gtest/gtest.h"
#include "src/cards.h"

namespace tarok {

static constexpr int kDealCardsAction = 0;
static constexpr int kBidTwoAction = 3;
static constexpr int kBidOneAction = 4;
static constexpr int kBidSoloTwoAction = 6;
static constexpr int kBidBeggarAction = 8;
static constexpr int kSoloWithoutAction = 9;
static constexpr int kBidOpenBeggarAction = 10;
static constexpr int kBidColourValatAction = 11;
static constexpr int kBidValatWithoutAction = 12;

class TarokStateTests : public ::testing::Test {
 protected:
  const std::array<Card, 54> deck_ = InitializeCardDeck();
};

}  // namespace tarok
