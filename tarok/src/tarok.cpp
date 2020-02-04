#include <iostream>

#include "open_spiel/spiel.h"
#include "pybind11/pybind11.h"

void printRegisteredGamesLikeTarok() {
    std::cout << "Registered games:" << std::endl;
    std::vector<std::string> names = open_spiel::RegisteredGames();
    for (const std::string& name : names) {
        std::cout << name << std::endl;
    }
}

PYBIND11_MODULE(open_tarok, m) {
    m.def(
        "printRegisteredGamesLikeTarok",
        &printRegisteredGamesLikeTarok,
        "Prints registered open spiel games"
    );
}
