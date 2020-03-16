/* Copyright 2020 Semantic Weights. All rights reserved. */

#include <algorithm>
#include <array>
#include <iterator>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include "src/cards.h"

namespace tarok {

Card::Card(Suit suit, int rank, int points, std::string short_name,
           std::string long_name)
    : suit_(suit),
      rank_(rank),
      points_(points),
      short_name_(short_name),
      long_name_(long_name) {}

bool Card::IsTrula() const { return suit_ == Suit::kTaroks && points_ == 5; }

std::string Card::ToString() const { return long_name_; }

std::array<Card, 54> BuildDeck() {
  static const std::array<Card, 54> deck = {
      // taroks
      Card(Suit::kTaroks, 8, 5, "T1", "Pagat"),
      Card(Suit::kTaroks, 9, 1, "T2", "II"),
      Card(Suit::kTaroks, 10, 1, "T3", "III"),
      Card(Suit::kTaroks, 11, 1, "T4", "IIII"),
      Card(Suit::kTaroks, 12, 1, "T5", "V"),
      Card(Suit::kTaroks, 13, 1, "T6", "VI"),
      Card(Suit::kTaroks, 14, 1, "T7", "VII"),
      Card(Suit::kTaroks, 15, 1, "T8", "VIII"),
      Card(Suit::kTaroks, 16, 1, "T9", "IX"),
      Card(Suit::kTaroks, 17, 1, "T10", "X"),
      Card(Suit::kTaroks, 18, 1, "T11", "XI"),
      Card(Suit::kTaroks, 19, 1, "T12", "XII"),
      Card(Suit::kTaroks, 20, 1, "T13", "XIII"),
      Card(Suit::kTaroks, 21, 1, "T14", "XIV"),
      Card(Suit::kTaroks, 22, 1, "T15", "XV"),
      Card(Suit::kTaroks, 23, 1, "T16", "XVI"),
      Card(Suit::kTaroks, 24, 1, "T17", "XVII"),
      Card(Suit::kTaroks, 25, 1, "T18", "XVIII"),
      Card(Suit::kTaroks, 26, 1, "T19", "XIX"),
      Card(Suit::kTaroks, 27, 1, "T20", "XX"),
      Card(Suit::kTaroks, 28, 5, "T21", "Mond"),
      Card(Suit::kTaroks, 29, 5, "T22", "Skis"),
      // hearts
      Card(Suit::kHearts, 0, 1, "H4", "4 of Hearts"),
      Card(Suit::kHearts, 1, 1, "H3", "3 of Hearts"),
      Card(Suit::kHearts, 2, 1, "H2", "2 of Hearts"),
      Card(Suit::kHearts, 3, 1, "H1", "1 of Hearts"),
      Card(Suit::kHearts, 4, 2, "HJ", "Jack of Hearts"),
      Card(Suit::kHearts, 5, 3, "HKN", "Knight of Hearts"),
      Card(Suit::kHearts, 6, 4, "HQ", "Queen of Hearts"),
      Card(Suit::kHearts, 7, 5, "HKI", "King of Hearts"),
      // diamonds
      Card(Suit::kDiamonds, 0, 1, "D4", "4 of Diamonds"),
      Card(Suit::kDiamonds, 1, 1, "D3", "3 of Diamonds"),
      Card(Suit::kDiamonds, 2, 1, "D2", "2 of Diamonds"),
      Card(Suit::kDiamonds, 3, 1, "D1", "1 of Diamonds"),
      Card(Suit::kDiamonds, 4, 2, "DJ", "Jack of Diamonds"),
      Card(Suit::kDiamonds, 5, 3, "DKN", "Knight of Diamonds"),
      Card(Suit::kDiamonds, 6, 4, "DQ", "Queen of Diamonds"),
      Card(Suit::kDiamonds, 7, 5, "DKI", "King of Diamonds"),
      // spades
      Card(Suit::kSpades, 0, 1, "S7", "7 of Spades"),
      Card(Suit::kSpades, 1, 1, "S8", "8 of Spades"),
      Card(Suit::kSpades, 2, 1, "S9", "9 of Spades"),
      Card(Suit::kSpades, 3, 1, "S10", "10 of Spades"),
      Card(Suit::kSpades, 4, 2, "SJ", "Jack of Spades"),
      Card(Suit::kSpades, 5, 3, "SKN", "Knight of Spades"),
      Card(Suit::kSpades, 6, 4, "SQ", "Queen of Spades"),
      Card(Suit::kSpades, 7, 5, "SKI", "King of Spades"),
      // clubs
      Card(Suit::kClubs, 0, 1, "C7", "7 of Clubs"),
      Card(Suit::kClubs, 1, 1, "C8", "8 of Clubs"),
      Card(Suit::kClubs, 2, 1, "C9", "9 of Clubs"),
      Card(Suit::kClubs, 3, 1, "C10", "10 of Clubs"),
      Card(Suit::kClubs, 4, 2, "CJ", "Jack of Clubs"),
      Card(Suit::kClubs, 5, 3, "CKN", "Knight of Clubs"),
      Card(Suit::kClubs, 6, 4, "CQ", "Queen of Clubs"),
      Card(Suit::kClubs, 7, 5, "CKI", "King of Clubs")};

  return deck;
}

std::tuple<std::vector<int>, std::array<std::vector<int>, 3>> DealCards(
    int seed = 42) {
  // create vector of card indices
  std::vector<int> cards(54);
  std::iota(std::begin(cards), std::end(cards), 0);

  // shuffle indices
  auto rng = std::default_random_engine(seed);
  std::shuffle(std::begin(cards), std::end(cards), rng);

  // first six cards are talon
  std::vector<int>::iterator start(cards.begin());
  std::vector<int>::iterator end(cards.begin() + 6);
  std::vector<int> talon(start, end);

  std::array<std::vector<int>, 3> private_cards{};
  // every player gets next 16 cards
  std::advance(start, 6);
  for (int i = 0; i < 3; i++) {
    std::advance(end, 16);
    private_cards[i] = std::vector<int>(start, end);
    std::advance(start, 16);
  }

  return {talon, private_cards};
}

}  // namespace tarok
