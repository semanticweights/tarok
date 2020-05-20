/* Copyright 2020 Semantic Weights. All rights reserved. */

#include "src/cards.h"

#include <algorithm>
#include <array>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include "open_spiel/spiel.h"

namespace tarok {

TarokCard::TarokCard(CardSuit suit, int rank, int points,
                     std::string short_name, std::string long_name)
    : suit(suit),
      rank(rank),
      points(points),
      short_name(short_name),
      long_name(long_name) {}

bool TarokCard::IsTrula() const {
  return suit == CardSuit::kTaroks && points == 5;
}

const std::string TarokCard::ToString() const { return long_name; }

const std::array<TarokCard, 54> InitializeCardDeck() {
  return {// taroks
          TarokCard(CardSuit::kTaroks, 8, 5, "T1", "Pagat"),
          TarokCard(CardSuit::kTaroks, 9, 1, "T2", "II"),
          TarokCard(CardSuit::kTaroks, 10, 1, "T3", "III"),
          TarokCard(CardSuit::kTaroks, 11, 1, "T4", "IIII"),
          TarokCard(CardSuit::kTaroks, 12, 1, "T5", "V"),
          TarokCard(CardSuit::kTaroks, 13, 1, "T6", "VI"),
          TarokCard(CardSuit::kTaroks, 14, 1, "T7", "VII"),
          TarokCard(CardSuit::kTaroks, 15, 1, "T8", "VIII"),
          TarokCard(CardSuit::kTaroks, 16, 1, "T9", "IX"),
          TarokCard(CardSuit::kTaroks, 17, 1, "T10", "X"),
          TarokCard(CardSuit::kTaroks, 18, 1, "T11", "XI"),
          TarokCard(CardSuit::kTaroks, 19, 1, "T12", "XII"),
          TarokCard(CardSuit::kTaroks, 20, 1, "T13", "XIII"),
          TarokCard(CardSuit::kTaroks, 21, 1, "T14", "XIV"),
          TarokCard(CardSuit::kTaroks, 22, 1, "T15", "XV"),
          TarokCard(CardSuit::kTaroks, 23, 1, "T16", "XVI"),
          TarokCard(CardSuit::kTaroks, 24, 1, "T17", "XVII"),
          TarokCard(CardSuit::kTaroks, 25, 1, "T18", "XVIII"),
          TarokCard(CardSuit::kTaroks, 26, 1, "T19", "XIX"),
          TarokCard(CardSuit::kTaroks, 27, 1, "T20", "XX"),
          TarokCard(CardSuit::kTaroks, 28, 5, "T21", "Mond"),
          TarokCard(CardSuit::kTaroks, 29, 5, "T22", "Skis"),
          // hearts
          TarokCard(CardSuit::kHearts, 0, 1, "H4", "4 of Hearts"),
          TarokCard(CardSuit::kHearts, 1, 1, "H3", "3 of Hearts"),
          TarokCard(CardSuit::kHearts, 2, 1, "H2", "2 of Hearts"),
          TarokCard(CardSuit::kHearts, 3, 1, "H1", "1 of Hearts"),
          TarokCard(CardSuit::kHearts, 4, 2, "HJ", "Jack of Hearts"),
          TarokCard(CardSuit::kHearts, 5, 3, "HKN", "Knight of Hearts"),
          TarokCard(CardSuit::kHearts, 6, 4, "HQ", "Queen of Hearts"),
          TarokCard(CardSuit::kHearts, 7, 5, "HKI", "King of Hearts"),
          // diamonds
          TarokCard(CardSuit::kDiamonds, 0, 1, "D4", "4 of Diamonds"),
          TarokCard(CardSuit::kDiamonds, 1, 1, "D3", "3 of Diamonds"),
          TarokCard(CardSuit::kDiamonds, 2, 1, "D2", "2 of Diamonds"),
          TarokCard(CardSuit::kDiamonds, 3, 1, "D1", "1 of Diamonds"),
          TarokCard(CardSuit::kDiamonds, 4, 2, "DJ", "Jack of Diamonds"),
          TarokCard(CardSuit::kDiamonds, 5, 3, "DKN", "Knight of Diamonds"),
          TarokCard(CardSuit::kDiamonds, 6, 4, "DQ", "Queen of Diamonds"),
          TarokCard(CardSuit::kDiamonds, 7, 5, "DKI", "King of Diamonds"),
          // spades
          TarokCard(CardSuit::kSpades, 0, 1, "S7", "7 of Spades"),
          TarokCard(CardSuit::kSpades, 1, 1, "S8", "8 of Spades"),
          TarokCard(CardSuit::kSpades, 2, 1, "S9", "9 of Spades"),
          TarokCard(CardSuit::kSpades, 3, 1, "S10", "10 of Spades"),
          TarokCard(CardSuit::kSpades, 4, 2, "SJ", "Jack of Spades"),
          TarokCard(CardSuit::kSpades, 5, 3, "SKN", "Knight of Spades"),
          TarokCard(CardSuit::kSpades, 6, 4, "SQ", "Queen of Spades"),
          TarokCard(CardSuit::kSpades, 7, 5, "SKI", "King of Spades"),
          // clubs
          TarokCard(CardSuit::kClubs, 0, 1, "C7", "7 of Clubs"),
          TarokCard(CardSuit::kClubs, 1, 1, "C8", "8 of Clubs"),
          TarokCard(CardSuit::kClubs, 2, 1, "C9", "9 of Clubs"),
          TarokCard(CardSuit::kClubs, 3, 1, "C10", "10 of Clubs"),
          TarokCard(CardSuit::kClubs, 4, 2, "CJ", "Jack of Clubs"),
          TarokCard(CardSuit::kClubs, 5, 3, "CKN", "Knight of Clubs"),
          TarokCard(CardSuit::kClubs, 6, 4, "CQ", "Queen of Clubs"),
          TarokCard(CardSuit::kClubs, 7, 5, "CKI", "King of Clubs")};
}

DealtCards DealCards(int num_players, int seed) {
  // create vector of card indices
  std::vector<open_spiel::Action> cards(54);
  std::iota(cards.begin(), cards.end(), 0);
  // shuffle indices
  std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));

  // first six cards are talon
  auto begin = cards.begin();
  auto end = begin + 6;
  std::vector<open_spiel::Action> talon(begin, end);

  // deal the rest of the cards to players
  int num_cards_per_player = 48 / num_players;
  std::vector<std::vector<open_spiel::Action>> players_cards;
  players_cards.reserve(num_players);

  std::advance(begin, 6);
  for (int i = 0; i < num_players; i++) {
    std::advance(end, num_cards_per_player);
    std::vector<open_spiel::Action> player_cards(begin, end);
    // player's cards are sorted since legal actions need to be returned in
    // ascending order
    std::sort(player_cards.begin(), player_cards.end());
    players_cards.push_back(player_cards);
    std::advance(begin, num_cards_per_player);
  }

  return {talon, players_cards};
}

}  // namespace tarok
