#include "tarok/tarok.h"
#include "gtest/gtest.h"

namespace tarok {

TEST(TarokGameTests, SomeTodoTest) {
    std::shared_ptr<const open_spiel::Game> game = tarok::NewTarokGame();
    EXPECT_EQ(game->NumDistinctActions(), 0);
}

} // namespace tarok
