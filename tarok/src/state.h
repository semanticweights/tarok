/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "open_spiel/spiel.h"
#include "src/cards.h"
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

std::ostream& operator<<(std::ostream& os, const GamePhase& game_phase);

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
  std::vector<std::vector<open_spiel::Action>> Talon() const;
  std::vector<open_spiel::Action> PlayerCards(open_spiel::Player player) const;
  std::string CardActionToString(open_spiel::Action action_id) const;
  Contract SelectedContract() const;

 protected:
  void DoApplyAction(open_spiel::Action action_id) override;

 private:
  std::vector<open_spiel::Action> LegalActionsInBidding() const;
  std::vector<open_spiel::Action> LegalActionsInTalonExchange() const;
  std::vector<open_spiel::Action> LegalActionsInTricksPlaying() const;
  std::vector<open_spiel::Action> LegalActionsInTricksPlayingOpening() const;
  std::vector<open_spiel::Action> LegalActionsInTricksPlayingFollowing() const;
  void DoApplyActionInCardDealing();
  void DoApplyActionInBidding(open_spiel::Action action_id);
  void FinishBiddingPhase(open_spiel::Action action_id);
  void DoApplyActionInKingCalling(open_spiel::Action action_id);
  void DoApplyActionInTalonExchange(open_spiel::Action action_id);
  void StartTricksPlayingPhase();
  bool AllButCurrentPlayerPassedBidding() const;
  void NextPlayer();
  static bool ActionInActions(open_spiel::Action action_id,
                              const std::vector<open_spiel::Action>& actions);
  static void MoveActionFromTo(open_spiel::Action action_id,
                               std::vector<open_spiel::Action>* from,
                               std::vector<open_spiel::Action>* to);
  std::vector<open_spiel::Action> RemovePagatIfNeeded(
      const std::vector<open_spiel::Action>& actions) const;
  // checks whether the current player can follow the opening card suit or
  // can't but still has at least one tarok, if the first value is true, the
  // second might be set incorrectly as it is irrelevant
  std::tuple<bool, bool> CanFollowSuitOrCantButHasTarok() const;
  std::vector<open_spiel::Action> TakeSuitFromPlayerCards(CardSuit suit) const;
  std::vector<open_spiel::Action> TakeSuitFromPlayerCardsHigherIfNeeded() const;
  const TarokCard& CardToBeatInNegativeContracts() const;

  std::shared_ptr<const TarokGame> tarok_parent_game_;
  GamePhase current_game_phase_ = GamePhase::kCardDealing;
  open_spiel::Player current_player_ = open_spiel::kInvalidPlayer;
  std::vector<open_spiel::Action> talon_;
  std::vector<std::vector<open_spiel::Action>> players_cards_;
  std::vector<open_spiel::Action> players_bids_;
  open_spiel::Player declarer_ = open_spiel::kInvalidPlayer;
  // contract pointed to is managed by the game instance
  const ContractInfo* selected_contract_;
  open_spiel::Player declarer_partner_ = open_spiel::kInvalidPlayer;
  std::vector<std::vector<open_spiel::Action>> players_collected_cards_;
  std::vector<open_spiel::Action> trick_cards_;
};

}  // namespace tarok
