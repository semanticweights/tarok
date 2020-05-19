/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/contracts.h"

namespace tarok {

ContractInfo::ContractInfo(Contract contract, std::string name, int score)
    : contract(contract), name(name), score(score) {}

const std::array<ContractInfo, 12> InitializeContracts() {
  return {
      ContractInfo(Contract::kKlop, "Klop", 70),
      ContractInfo(Contract::kThree, "Three", 10),
      ContractInfo(Contract::kTwo, "Two", 20),
      ContractInfo(Contract::kOne, "One", 30),
      ContractInfo(Contract::kSoloThree, "Solo three", 40),
      ContractInfo(Contract::kSoloTwo, "Solo two", 50),
      ContractInfo(Contract::kSoloOne, "Solo one", 60),
      ContractInfo(Contract::kBeggar, "Beggar", 70),
      ContractInfo(Contract::kSoloWithout, "Solo without", 80),
      ContractInfo(Contract::kOpenBeggar, "Open beggar", 90),
      ContractInfo(Contract::kColourValatWithout, "Colour valat without", 125),
      ContractInfo(Contract::kValatWithout, "Valat without", 500)};
}

}  // namespace tarok
