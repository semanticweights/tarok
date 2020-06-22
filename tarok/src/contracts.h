/* Copyright 2020 Semantic Weights. All rights reserved. */

#pragma once

#include <array>
#include <iostream>
#include <string>

#include "open_spiel/spiel.h"

namespace tarok {

// a subset of bidding actions that are used throughout codebase and add to
// readability
static constexpr int kInvalidContractBid = -1;
static constexpr int kBidPass = 0;
static constexpr int kBidKlop = 1;
static constexpr int kBidThree = 2;
static constexpr int kBidSoloThree = 5;
static constexpr int kBidSoloOne = 7;

enum class ContractName {
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

struct Contract {
  Contract(ContractName name, int score, int num_talon_exchanges,
           bool needs_king_calling, bool declarer_starts, bool is_negative);

  bool NeedsTalonExchange() const;

  const ContractName name;
  const int score;
  const int num_talon_exchanges;
  const bool needs_king_calling;
  const bool declarer_starts;
  const bool is_negative;
};

const std::array<Contract, 12> InitializeContracts();

std::ostream& operator<<(std::ostream& os, const ContractName& contract_name);

std::string ContractNameToString(const ContractName& contract_name);

}  // namespace tarok
