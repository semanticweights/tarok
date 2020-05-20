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

// biddable contracts, note that these are actions that are offset by +1
// wrt. InitializeContracts() since action 0 means pass
// 3 player game
static constexpr std::array<int, 5> kBiddableContracts3 = {2, 3, 4, 8, 9};
// 4 player game excluding the forehand
static constexpr std::array<int, 10> kBiddableContracts4 = {3, 4, 5,  6,  7,
                                                            8, 9, 10, 11, 12};

struct ContractInfo {
  ContractInfo(Contract contract, std::string name, int score);

  const Contract contract;
  const std::string name;
  const int score;
};

const std::array<ContractInfo, 12> InitializeContracts();

}  // namespace tarok
