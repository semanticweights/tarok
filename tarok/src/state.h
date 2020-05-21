/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "open_spiel/spiel.h"
#include "src/contracts.h"

namespace tarok {

enum class GamePhase {
  kCardDealing,
  kBidding,
  kKingCalling,
  kTalonExchange,
  kTricksPlaying,
  kFinished
};

class TarokGame;

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
  GamePhase CurrentGamePhase() const;
  std::vector<std::string> Talon() const;
  std::vector<std::string> PlayerCards(open_spiel::Player player) const;
  Contract SelectedContract() const;

 protected:
  void DoApplyAction(open_spiel::Action action_id) override;

 private:
  std::vector<open_spiel::Action> LegalActionsInBidding() const;
  void AddLegalActionsInBidding3(
      int max_bid, int max_bid_player,
      std::vector<open_spiel::Action>* result_actions) const;
  void AddLegalActionsInBidding4(
      int max_bid, int max_bid_player,
      std::vector<open_spiel::Action>* result_actions) const;
  void DoApplyActionInCardDealing();
  void DoApplyActionInBidding(open_spiel::Action action_id);
  void FinishBiddingPhase(open_spiel::Action action_id);
  bool AllButCurrentPlayerPassedBidding() const;
  void NextPlayer();

  std::shared_ptr<const TarokGame> tarok_parent_game_;
  GamePhase current_game_phase_;
  open_spiel::Player current_player_;
  std::vector<open_spiel::Action> talon_;
  std::vector<std::vector<open_spiel::Action>> players_cards_;
  std::vector<open_spiel::Action> players_bids_;
  open_spiel::Player declarer_;
  // contract pointed to is managed by the game instance
  const ContractInfo* selected_contract_;
};

}  // namespace tarok
