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

class TarokGame;

using TrickWinnerAndAction = std::tuple<open_spiel::Player, open_spiel::Action>;
using CollectedCardsPerTeam = std::tuple<std::vector<open_spiel::Action>,
                                         std::vector<open_spiel::Action>>;

class TarokState : public open_spiel::State {
 public:
  explicit TarokState(std::shared_ptr<const open_spiel::Game> game);

  open_spiel::Player CurrentPlayer() const override;
  bool IsTerminal() const override;
  GamePhase CurrentGamePhase() const;
  std::vector<open_spiel::Action> PlayerCards(open_spiel::Player player) const;
  ContractName SelectedContractName() const;
  std::vector<open_spiel::Action> Talon() const;
  std::vector<std::vector<open_spiel::Action>> TalonSets() const;
  std::vector<open_spiel::Action> TrickCards() const;

  std::vector<open_spiel::Action> LegalActions() const override;
  std::string ActionToString(open_spiel::Player player,
                             open_spiel::Action action_id) const override;
  std::string CardActionToString(open_spiel::Action action_id) const;
  open_spiel::ActionsAndProbs ChanceOutcomes() const override;

  // calculates the overall score for a finished game without radli, see
  // comments above CapturedMondPenalties() for more details
  std::vector<double> Returns() const override;
  // the following two methods are kept separately due to the captured mond
  // penalty not being affected by any multipliers for kontras or radli, note
  // that TarokState does not implement radli as they are, like cumulative
  // players' score, part of the global state that would have to be kept between
  // multiple NewInitialState() calls (i.e. TarokState only implements a single
  // round of the game and radli implementation is left to the owner of the game
  // instance who should keep track of multiple rounds if needed)
  std::vector<int> CapturedMondPenalties() const;
  std::vector<int> ScoresWithoutCapturedMondPenalties() const;

  // info state strings are of the following format (cards and actions are
  // delimited by a comma character, some parts of the string are omitted in
  // states where corresponding gamne phases are not played):
  //
  // each_players_private_cards;bidding_actions;king_calling_action;
  // talon_cards;choosing_talon_set_action;discarding_cards_actions;
  // single_trick_played_actions;...;single_trick_played_actions
  std::string InformationStateString(open_spiel::Player player) const override;

  std::string ToString() const override;
  std::unique_ptr<State> Clone() const override;

 protected:
  void DoApplyAction(open_spiel::Action action_id) override;

 private:
  std::vector<open_spiel::Action> LegalActionsInBidding() const;
  std::vector<open_spiel::Action> LegalActionsInTalonExchange() const;
  std::vector<open_spiel::Action> LegalActionsInTricksPlaying() const;
  std::vector<open_spiel::Action> LegalActionsInTricksPlayingFollowing() const;

  // checks whether the current player can follow the opening card suit or
  // can't but still has at least one tarok, if the first value is true, the
  // second might be set incorrectly as it is irrelevant
  std::tuple<bool, bool> CanFollowSuitOrCantButHasTarok() const;

  std::vector<open_spiel::Action> TakeSuitFromPlayerCardsInNegativeContracts(
      CardSuit suit) const;
  std::optional<open_spiel::Action> ActionToBeatInNegativeContracts(
      CardSuit suit) const;
  std::vector<open_spiel::Action> RemovePagatIfNeeded(
      const std::vector<open_spiel::Action>& actions) const;
  std::vector<open_spiel::Action> TakeSuitFromPlayerCardsInPositiveContracts(
      CardSuit suit) const;

  void DoApplyActionInCardDealing();
  bool AnyPlayerWithoutTaroks() const;
  void DoApplyActionInBidding(open_spiel::Action action_id);
  bool AllButCurrentPlayerPassedBidding() const;
  void FinishBiddingPhase(open_spiel::Action action_id);
  void DoApplyActionInKingCalling(open_spiel::Action action_id);
  void DoApplyActionInTalonExchange(open_spiel::Action action_id);
  void StartTricksPlayingPhase();
  void DoApplyActionInTricksPlaying(open_spiel::Action action_id);
  void ResolveTrick();
  TrickWinnerAndAction ResolveTrickWinnerAndWinningAction() const;

  // computes which player belongs to the trick_cards_ index as the player
  // who opens the trick always belongs to index 0 within trick_cards_
  open_spiel::Player TrickCardsIndexToPlayer(int index) const;

  std::vector<int> ScoresInKlop() const;
  std::vector<int> ScoresInNormalContracts() const;
  CollectedCardsPerTeam SplitCollectedCardsPerTeams() const;
  int NonValatBonuses(
      const std::vector<open_spiel::Action>& collected_cards,
      const std::vector<open_spiel::Action>& opposite_collected_cards) const;
  std::tuple<bool, bool> CollectedKingsAndOrTrula(
      const std::vector<open_spiel::Action>& collected_cards) const;
  std::vector<int> ScoresInHigherContracts() const;

  void NextPlayer();
  static bool ActionInActions(open_spiel::Action action_id,
                              const std::vector<open_spiel::Action>& actions);
  static void MoveActionFromTo(open_spiel::Action action_id,
                               std::vector<open_spiel::Action>* from,
                               std::vector<open_spiel::Action>* to);
  const Card& ActionToCard(open_spiel::Action action_id) const;
  void AppendToAllInformationStates(const std::string& appendix);
  void AppendToInformationState(open_spiel::Player player,
                                const std::string& appendix);

  std::shared_ptr<const TarokGame> tarok_parent_game_;
  GamePhase current_game_phase_ = GamePhase::kCardDealing;
  open_spiel::Player current_player_ = open_spiel::kInvalidPlayer;
  std::vector<open_spiel::Action> talon_;
  std::vector<std::vector<open_spiel::Action>> players_cards_;
  std::vector<open_spiel::Action> players_bids_;
  open_spiel::Player declarer_ = open_spiel::kInvalidPlayer;
  // contract pointed to is managed by the game instance
  const Contract* selected_contract_;
  open_spiel::Action called_king_ = open_spiel::kInvalidAction;
  bool called_king_in_talon_ = false;
  open_spiel::Player declarer_partner_ = open_spiel::kInvalidPlayer;
  std::vector<std::vector<open_spiel::Action>> players_collected_cards_;
  std::vector<open_spiel::Action> trick_cards_;
  open_spiel::Player captured_mond_player_ = open_spiel::kInvalidPlayer;
  std::vector<std::string> players_info_states_;
};

std::ostream& operator<<(std::ostream& os, const GamePhase& game_phase);

std::string GamePhaseToString(const GamePhase& game_phase);

}  // namespace tarok
