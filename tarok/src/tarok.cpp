#include <iostream>

#include "open_spiel/spiel.h"
#include "pybind11/pybind11.h"

namespace py = pybind11;

std::string openSpielGame() {
    return open_spiel::RegisteredGames()[0];
}

PYBIND11_MODULE(open_tarok, m) {
    m.def(
        "open_spiel_game",
        &openSpielGame,
        "Returns the first registered open spiel game"
    );
}
