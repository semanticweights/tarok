/* Copyright 2020 Semantic Weights. All rights reserved. */

// #include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "test/tarok_utils.h"

namespace tarok {

TEST(TarokStateTests, TestTricksPlayingPhase) {
  auto state = StateAfterActions(open_spiel::GameParameters(), {0, 0, 0, 2});
}

}  // namespace tarok
