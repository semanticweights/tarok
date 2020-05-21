/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/contracts.h"

namespace tarok {

ContractInfo::ContractInfo(Contract contract, std::string name, int score,
                           int num_talon_exchanges, bool needs_king_calling,
                           bool declarer_starts)
    : contract(contract),
      name(name),
      score(score),
      num_talon_exchanges(num_talon_exchanges),
      needs_king_calling(needs_king_calling),
      declarer_starts(declarer_starts) {}

bool ContractInfo::NeedsTalonExchange() const {
  return num_talon_exchanges > 0;
}

const std::array<ContractInfo, 12> InitializeContracts() {
  return {
      ContractInfo(Contract::kKlop, "Klop", 70, 0, false, false),
      ContractInfo(Contract::kThree, "Three", 10, 3, true, false),
      ContractInfo(Contract::kTwo, "Two", 20, 2, true, false),
      ContractInfo(Contract::kOne, "One", 30, 1, true, false),
      ContractInfo(Contract::kSoloThree, "Solo three", 40, 3, false, false),
      ContractInfo(Contract::kSoloTwo, "Solo two", 50, 2, false, false),
      ContractInfo(Contract::kSoloOne, "Solo one", 60, 1, false, false),
      ContractInfo(Contract::kBeggar, "Beggar", 70, 0, false, true),
      ContractInfo(Contract::kSoloWithout, "Solo without", 80, 0, false, true),
      ContractInfo(Contract::kOpenBeggar, "Open beggar", 90, 0, false, true),
      ContractInfo(Contract::kColourValatWithout, "Colour valat without", 125,
                   0, false, true),
      ContractInfo(Contract::kValatWithout, "Valat without", 500, 0, false,
                   true)};
}

}  // namespace tarok
