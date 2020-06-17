/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/contracts.h"

namespace tarok {

ContractInfo::ContractInfo(Contract contract, std::string name, int score,
                           int num_talon_exchanges, bool needs_king_calling,
                           bool declarer_starts, bool is_negative)
    : contract(contract),
      name(name),
      score(score),
      num_talon_exchanges(num_talon_exchanges),
      needs_king_calling(needs_king_calling),
      declarer_starts(declarer_starts),
      is_negative(is_negative) {}

bool ContractInfo::NeedsTalonExchange() const {
  return num_talon_exchanges > 0;
}

const std::array<ContractInfo, 12> InitializeContracts() {
  return {
      ContractInfo(Contract::kKlop, "Klop", 70, 0, false, false, true),
      ContractInfo(Contract::kThree, "Three", 10, 3, true, false, false),
      ContractInfo(Contract::kTwo, "Two", 20, 2, true, false, false),
      ContractInfo(Contract::kOne, "One", 30, 1, true, false, false),
      ContractInfo(Contract::kSoloThree, "Solo three", 40, 3, false, false,
                   false),
      ContractInfo(Contract::kSoloTwo, "Solo two", 50, 2, false, false, false),
      ContractInfo(Contract::kSoloOne, "Solo one", 60, 1, false, false, false),
      ContractInfo(Contract::kBeggar, "Beggar", 70, 0, false, true, true),
      ContractInfo(Contract::kSoloWithout, "Solo without", 80, 0, false, true,
                   false),
      ContractInfo(Contract::kOpenBeggar, "Open beggar", 90, 0, false, true,
                   true),
      ContractInfo(Contract::kColourValatWithout, "Colour valat without", 125,
                   0, false, true, false),
      ContractInfo(Contract::kValatWithout, "Valat without", 500, 0, false,
                   true, false)};
}

std::ostream& operator<<(std::ostream& os, const Contract& contract) {
  os << ContractToString(contract);
  return os;
}

std::string ContractToString(const Contract& contract) {
  std::string str = "Contract::";
  switch (contract) {
    case Contract::kKlop:
      absl::StrAppend(&str, "Klop");
      break;
    case Contract::kThree:
      absl::StrAppend(&str, "Three");
      break;
    case Contract::kTwo:
      absl::StrAppend(&str, "Two");
      break;
    case Contract::kOne:
      absl::StrAppend(&str, "One");
      break;
    case Contract::kSoloThree:
      absl::StrAppend(&str, "SoloThree");
      break;
    case Contract::kSoloTwo:
      absl::StrAppend(&str, "SoloTwo");
      break;
    case Contract::kSoloOne:
      absl::StrAppend(&str, "SoloOne");
      break;
    case Contract::kBeggar:
      absl::StrAppend(&str, "Beggar");
      break;
    case Contract::kSoloWithout:
      absl::StrAppend(&str, "SoloWithout");
      break;
    case Contract::kOpenBeggar:
      absl::StrAppend(&str, "OpenBeggar");
      break;
    case Contract::kColourValatWithout:
      absl::StrAppend(&str, "ColourValatWithout");
      break;
    case Contract::kValatWithout:
      absl::StrAppend(&str, "ValatWithout");
      break;
    case Contract::kNotSelected:
      absl::StrAppend(&str, "NotSelected");
  }
  return str;
}

}  // namespace tarok
