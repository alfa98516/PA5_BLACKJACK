#include "Hand.hpp"
#include <atomic>
#include <cstdint>

Hand::Hand(std::shared_ptr<Deck> _deck) : deck(_deck), hand(), Ace(false), BlackJack(false) {
    Deck::Card c1 = deck->Draw();
    Deck::Card c2 = deck->Draw();
    switch (c1.GetRank()) {
    case Rank::Ace:
        score += 11;
        Ace = true;
        break;
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        score += 10;
        break;
    default:
        score += (int8_t)c1.GetRank();
        break;
    }

    switch (c2.GetRank()) {
    case Rank::Ace:

        if (!Ace)
            score += 11;
        else {
            score += 1;
            break;
        }

        Ace = true;
        break;
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        score += 10;
        break;
    default:
        score += (int8_t)c2.GetRank();
        break;
    }

    if (Ace && score == 21) {
        BlackJack = true;
    }
}

const std::vector<Deck::Card>& Hand::GetHand() const { return hand; }

int8_t Hand::GetScore() const { return score; }

void Hand::Hit() {
    if (score == -1) return;
    Deck::Card c = deck->Draw();
    switch (c.GetRank()) {
    case Rank::Ace:

        if (!Ace)
            score += 11;
        else {
            score += 1;
            break;
        }

        Ace = true;
        break;
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        score += 10;
        break;
    default:
        score += (int8_t)c.GetRank();
        break;
    }

    if (score > 21) {
        if (Ace) {
            score -= 10;
            if (score <= 21) return;
        }
        score = -1;
    }
}
