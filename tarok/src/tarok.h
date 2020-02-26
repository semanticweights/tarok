/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "open_spiel/spiel.h"

namespace tarok {

// instantiate the game instance via a shared_ptr
// (see Game declaration comments in open_spiel/spiel.h)
std::shared_ptr<const open_spiel::Game> NewTarokGame(
    const open_spiel::GameParameters& params);

// game declaration
class TarokGame : public open_spiel::Game {
 public:
  explicit TarokGame(const open_spiel::GameParameters& params);

  int NumDistinctActions() const override;
  std::unique_ptr<open_spiel::State> NewInitialState() const override;
  // int MaxChanceOutcomes() const override;
  int NumPlayers() const override;
  double MinUtility() const override;
  double MaxUtility() const override;
  std::shared_ptr<const Game> Clone() const override;
  // double UtilitySum() const override;
  int MaxGameLength() const override;
};

// state declaration
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
};

}  // namespace tarok
