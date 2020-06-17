/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <array>
#include <iostream>
#include <string>

#include "open_spiel/spiel.h"

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
  kValatWithout,
  kNotSelected
};

struct ContractInfo {
  ContractInfo(Contract contract, std::string name, int score,
               int num_talon_exchanges, bool needs_king_calling,
               bool declarer_starts, bool is_negative);

  bool NeedsTalonExchange() const;

  const Contract contract;
  const std::string name;
  const int score;
  const int num_talon_exchanges;
  const bool needs_king_calling;
  const bool declarer_starts;
  const bool is_negative;
};

const std::array<ContractInfo, 12> InitializeContracts();

std::ostream& operator<<(std::ostream& os, const Contract& contract);

std::string ContractToString(const Contract& contract);

}  // namespace tarok
