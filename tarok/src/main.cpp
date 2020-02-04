#include <iostream>

#include "open_spiel/spiel.h"

int main() {
    std::cerr << "Registered games:" << std::endl;
    std::vector<std::string> names = open_spiel::RegisteredGames();
    for (const std::string& name : names) {
        std::cerr << name << std::endl;
    }
}
