#include "tarok.h"
#include "gtest/gtest.h"

namespace tarok {

TEST(TarokGameTests, TestNumDistinctActions) {
  std::shared_ptr<const open_spiel::Game> game = tarok::NewTarokGame(
      open_spiel::GameParameters()
  );
  EXPECT_EQ(game->NumDistinctActions(), 0);
}

} // namespace tarok
