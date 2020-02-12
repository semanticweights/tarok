#include "tarok.h"
#include "open_spiel/spiel.h"

namespace tarok {

// instantiate the game instance via a shared_ptr
// (see Game declaration comments in open_spiel/spiel.h)
std::shared_ptr<const open_spiel::Game> NewTarokGame(
    const open_spiel::GameParameters& params
) {
  return std::shared_ptr<const open_spiel::Game>(new TarokGame(params));
}

// game facts
const open_spiel::GameType kGameType{
    "tarok", // short_name
    "Slovenian Tarok", // long_name
    open_spiel::GameType::Dynamics::kSequential,
    open_spiel::GameType::ChanceMode::kExplicitStochastic,
    open_spiel::GameType::Information::kImperfectInformation,
    open_spiel::GameType::Utility::kGeneralSum,
    open_spiel::GameType::RewardModel::kTerminal,
    3, // max_num_players
    3, // min_num_players
    true, // provides_information_state_string
    false, // provides_information_state_tensor
    false, // provides_observation_string
    false, // provides_observation_tensor
    {} // parameter_specification
};

// game definition
TarokGame::TarokGame(const open_spiel::GameParameters& params)
    : Game(kGameType, params) {}

int TarokGame::NumDistinctActions() const {
    return 0;
}

std::unique_ptr<open_spiel::State> TarokGame::NewInitialState() const {
    return nullptr;
}

int TarokGame::NumPlayers() const {
    return 3;
}

double TarokGame::MinUtility() const {
    return 0.0;
}

double TarokGame::MaxUtility() const {
    return 0.0;
}

std::shared_ptr<const open_spiel::Game> TarokGame::Clone() const {
    return nullptr;
}

int TarokGame::MaxGameLength() const {
    return 0;
}

// state definition
TarokState::TarokState(std::shared_ptr<const open_spiel::Game> game)
    : open_spiel::State(game) {}

open_spiel::Player TarokState::CurrentPlayer() const {
    return -1;
}

std::vector<open_spiel::Action> TarokState::LegalActions() const {
    return std::vector<open_spiel::Action> {};
}

std::string TarokState::ActionToString(
    open_spiel::Player player,
    open_spiel::Action action_id
) const {
    return "";
}

open_spiel::Action TarokState::StringToAction(
    open_spiel::Player player,
    const std::string& action_str
) const {
    return open_spiel::Action();
}

std::string TarokState::ToString() const {
    return "";
}

bool TarokState::IsTerminal() const {
    return true;
}

std::vector<double> TarokState::Returns() const {
    return std::vector<double> {};
}

std::string TarokState::InformationStateString(
    open_spiel::Player player
) const {
    return "";
}

std::unique_ptr<open_spiel::State> TarokState::Clone() const {
    return nullptr;
}

open_spiel::ActionsAndProbs TarokState::ChanceOutcomes() const {
    return open_spiel::ActionsAndProbs();
}

void TarokState::DoApplyAction(open_spiel::Action action_id) {}

} // namespace tarok
