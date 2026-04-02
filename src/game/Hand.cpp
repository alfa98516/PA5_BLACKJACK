#include "Hand.hpp"
#include <cstdint>
#include <iostream>
#include <memory>

Hand::Hand(std::shared_ptr<Deck> _deck)
    : deck(_deck), hand(std::make_shared<std::vector<Deck::Card> >()), score(0), Ace(false),
      BlackJack(false), stand(false) {
    std::shared_ptr<Deck::Card> c1 =
        deck->Draw(); // std::make_shared<Deck::Card>(Suits::Club, Rank::Ace);
    std::shared_ptr<Deck::Card> c2 =
        deck->Draw(); // std::make_shared<Deck::Card>(Suits::Spade, Rank::Ace);
    hand->push_back(*c1);
    hand->push_back(*c2);
    switch (c1->GetRank()) {
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
            score += (int32_t)c1->GetRank();
            break;
    }

    switch (c2->GetRank()) {
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
            score += (int32_t)c2->GetRank();
            break;
    }

    if (Ace && score == 21) {
        BlackJack = true;
    }
}

/*
 * @breif Intended for debugging purposes, its so i can make mine and the dealers hand anything upon
 * game start.
 */
Hand::Hand(std::shared_ptr<Deck::Card> c1, std::shared_ptr<Deck::Card> c2,
           std::shared_ptr<Deck> _deck)
    : deck(_deck), score(0), Ace(false), BlackJack(false), stand(false),
      hand(std::make_shared<std::vector<Deck::Card> >()) {
    std::cout << "It might be the hand initializer?\n";
    hand->push_back(*c1);
    hand->push_back(*c2);
    std::cout << "it is not\n";

    switch (c1->GetRank()) {
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
            score += (int32_t)c1->GetRank();
            break;
    }

    switch (c2->GetRank()) {
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
            score += (int32_t)c2->GetRank();
            break;
    }

    if (Ace && score == 21) {
        std::cout << "Blackjack\n";
        BlackJack = true;
    }
    std::cout << "or maybe it is?\n";
}

const std::shared_ptr<std::vector<Deck::Card> > Hand::GetHand() const { return hand; }

int32_t Hand::GetScore() const { return score; }

void Hand::Stand() { stand = true; }

const std::shared_ptr<Deck::Card> Hand::Hit() {
    std::shared_ptr<Deck::Card> c = deck->Draw();
    hand->push_back(*c);
    switch (c->GetRank()) {
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
            score += (int32_t)c->GetRank();
            break;
    }

    if (score > 21) {
        if (Ace) {
            score -= 10;
            Ace = false;
            if (score <= 21)
                return c;
        }
        score = -1;
    }

    return c;
}
