#include "tarok.h"
#include "gtest/gtest.h"

namespace tarok {

TEST(TarokGameTests, TestNumDistinctActions) {
    open_spiel::Game* game = new tarok::TarokGame(open_spiel::GameParameters());
    EXPECT_EQ(game->NumDistinctActions(), 0);
}

} // namespace tarok
