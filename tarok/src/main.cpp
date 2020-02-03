#include <iostream>

#include "open_spiel/spiel.h"

int main() {
    // Print out registered games.
    int a = 0;
    std::cerr << "Registered gamesss:" << std::endl;
    std::vector<std::string> names = open_spiel::RegisteredGames();
    for (const std::string& name : names) {
        std::cerr << name << std::endl;
    }
}
