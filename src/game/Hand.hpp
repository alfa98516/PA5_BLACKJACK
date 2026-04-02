#pragma once
#include "Deck.hpp"
#include <cstdint>
#include <memory>
#include <vector>
class Hand {
    std::shared_ptr<std::vector<Deck::Card> > hand;
    int32_t score;
    std::shared_ptr<Deck> deck;
    bool Ace;
    bool BlackJack;
    bool stand;

  public:
    Hand(std::shared_ptr<Deck> _deck);
    Hand(std::shared_ptr<Deck::Card> c1, std::shared_ptr<Deck::Card> c2,
         std::shared_ptr<Deck> _deck);
    const std::shared_ptr<std::vector<Deck::Card> > GetHand() const;
    int32_t GetScore() const;
    const std::shared_ptr<Deck::Card> Hit();
    void Stand();

    bool IsBlackJack() const { return BlackJack; }
    bool IsBust() const { return score == -1; }
    bool IsStand() const { return stand; }
    bool operator==(const Hand& other) const { return score == other.GetScore(); }
    bool operator>(const Hand& other) const { return score > other.GetScore(); }
};
