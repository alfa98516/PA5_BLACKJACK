#pragma once
#include "Deck.hpp"
#include <cstdint>
#include <memory>
#include <vector>
class Hand {
    std::vector<Deck::Card> hand;
    int8_t score;
    std::shared_ptr<Deck> deck;
    bool Ace;
    bool BlackJack;

  public:
    bool IsBlackJack() const { return BlackJack; }
    Hand(std::shared_ptr<Deck> _deck);
    const std::vector<Deck::Card>& GetHand() const;
    int8_t GetScore() const;
    void Hit();
    bool operator==(const Hand& other) { return score == other.GetScore(); }
};
