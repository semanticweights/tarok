/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "open_spiel/spiel.h"
#include "src/game.h"

namespace tarok {

class TarokState : public open_spiel::State {
 public:
  explicit TarokState(std::shared_ptr<const open_spiel::Game> game);

  open_spiel::Player CurrentPlayer() const override;
  std::vector<open_spiel::Action> LegalActions() const override;
  std::string ActionToString(open_spiel::Player player,
                             open_spiel::Action action_id) const override;
  open_spiel::Action StringToAction(
      open_spiel::Player player, const std::string& action_str) const override;
  std::string ToString() const override;
  bool IsTerminal() const override;
  std::vector<double> Returns() const override;
  std::string InformationStateString(open_spiel::Player player) const override;
  std::unique_ptr<State> Clone() const override;
  open_spiel::ActionsAndProbs ChanceOutcomes() const override;

 protected:
  void DoApplyAction(open_spiel::Action action_id) override;

 private:
  const TarokGame& parent_game_;
  std::vector<int> talon_;
  std::array<std::vector<int>, 3> players_cards_;
};

}  // namespace tarok
