#include "tarok.h"

#include "open_spiel/spiel.h"

namespace tarok {

const open_spiel::GameType kGameType{
    /*short_name=*/"tarok",
    /*long_name=*/"Tarok",
    open_spiel::GameType::Dynamics::kSequential,
    open_spiel::GameType::ChanceMode::kExplicitStochastic,
    open_spiel::GameType::Information::kImperfectInformation,
    open_spiel::GameType::Utility::kGeneralSum,
    open_spiel::GameType::RewardModel::kTerminal,
    /*max_num_players=*/3,
    /*min_num_players=*/3,
    /*provides_information_state_string=*/true,
    /*provides_information_state_tensor=*/false,
    /*provides_observation_string=*/false,
    /*provides_observation_tensor=*/false,
    /*parameter_specification=*/{}
};

std::shared_ptr<const open_spiel::Game> Factory(
    const open_spiel::GameParameters& params
) {
  return std::shared_ptr<const open_spiel::Game>(new TarokGame(params));
}

open_spiel::REGISTER_SPIEL_GAME(kGameType, Factory);

// TarokGame::TarokGame(const open_spiel::GameParameters& params) : open_spiel::Game(params) {}

} // namespace tarok
