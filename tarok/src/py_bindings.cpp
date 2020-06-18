/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <memory>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "src/game.h"

namespace tarok {

namespace py = pybind11;

PYBIND11_MODULE(pytarok, m) {
  py::module::import("pyspiel");

  // game object
  py::class_<TarokGame, open_spiel::Game, std::shared_ptr<TarokGame>>
      tarok_game(m, "TarokGame");
  // game object constructor
  tarok_game.def(py::init([](const open_spiel::GameParameters& params) {
    // instantiate the game instance via a shared_ptr, see game declaration
    // comments in open_spiel/spiel.h for more info
    return std::make_shared<TarokGame>(params);
  }));

  // state object
  py::class_<TarokState, open_spiel::State> tarok_state(m, "TarokState");
  tarok_state.def("card_action_to_string", &TarokState::CardActionToString);
  tarok_state.def("current_game_phase", &TarokState::CurrentGamePhase);
  tarok_state.def("player_cards", &TarokState::PlayerCards);
  tarok_state.def("selected_contract", &TarokState::SelectedContract);
  tarok_state.def("talon", &TarokState::Talon);
  tarok_state.def("talon_sets", &TarokState::TalonSets);
  tarok_state.def("trick_cards", &TarokState::TrickCards);
  tarok_state.def("captured_mond_penalties",
                  &TarokState::CapturedMondPenalties);
  tarok_state.def("scores_without_captured_mond_penalties",
                  &TarokState::ScoresWithoutCapturedMondPenalties);

  // game phase object
  py::enum_<GamePhase> game_phase(m, "GamePhase");
  game_phase.value("CARD_DEALING", GamePhase::kCardDealing);
  game_phase.value("BIDDING", GamePhase::kBidding);
  game_phase.value("KING_CALLING", GamePhase::kKingCalling);
  game_phase.value("TALON_EXCHANGE", GamePhase::kTalonExchange);
  game_phase.value("TRICKS_PLAYING", GamePhase::kTricksPlaying);
  game_phase.value("FINISHED", GamePhase::kFinished);

  // contract object
  py::enum_<Contract> contract(m, "Contract");
  contract.value("KLOP", Contract::kKlop);
  contract.value("THREE", Contract::kThree);
  contract.value("TWO", Contract::kTwo);
  contract.value("ONE", Contract::kOne);
  contract.value("SOLO_THREE", Contract::kSoloThree);
  contract.value("SOLO_TWO", Contract::kSoloTwo);
  contract.value("SOLO_ONE", Contract::kSoloOne);
  contract.value("BEGGAR", Contract::kBeggar);
  contract.value("SOLO_WITHOUT", Contract::kSoloWithout);
  contract.value("OPEN_BEGGAR", Contract::kOpenBeggar);
  contract.value("COLOUR_VALAT_WITHOUT", Contract::kColourValatWithout);
  contract.value("VALAT_WITHOUT", Contract::kValatWithout);
  contract.value("NOT_SELECTED", Contract::kNotSelected);
}

}  // namespace tarok
