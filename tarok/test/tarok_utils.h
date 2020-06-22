/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <memory>
#include <vector>

#include "src/game.h"

namespace tarok {

std::unique_ptr<TarokState> StateAfterActions(
    const open_spiel::GameParameters& params,
    const std::vector<open_spiel::Action>& actions);

bool AllActionsInOtherActions(
    const std::vector<open_spiel::Action>& actions,
    const std::vector<open_spiel::Action>& other_actions);

open_spiel::Action ActionFromCardName(std::string short_name,
                                      const std::array<Card, 54>& deck);

}  // namespace tarok
