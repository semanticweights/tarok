/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <array>
#include <string>

namespace tarok {

enum class Contract {
  kKlop,
  kThree,
  kTwo,
  kOne,
  kSoloThree,
  kSoloTwo,
  kSoloOne,
  kBeggar,
  kSoloWithout,
  kOpenBeggar,
  kColourValatWithout,
  kValatWithout
};

struct ContractInfo {
  ContractInfo(Contract contract, std::string name, int score);

  const Contract contract;
  const std::string name;
  const int score;
};

const std::array<ContractInfo, 12> InitializeContracts();

}  // namespace tarok
