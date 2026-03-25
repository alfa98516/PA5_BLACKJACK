#include "Deck.hpp"
#include "Macros.hpp"
#include <algorithm>
#include <random>

std::random_device rd;

Deck::Deck(bool shuffle) : deck() {
    for (int _rank = 0; _rank < 13; ++_rank) {
        for (int _suit = 0; _suit < 4; ++_suit) {
            deck.push_back(Card((Suits)_suit, (Rank)_rank));
        }
    }
    if (shuffle) {
        Shuffle();
    }
}

Deck::Card Deck::Draw() {
    Card card = deck.back();
    deck.pop_back();
    return card;
}

void Deck::Shuffle() { std::shuffle(deck.begin(), deck.end(), std::mt19937(rd())); }

Deck::Card Deck::Peek() const { return deck.back(); }

Deck::Card::Card(const Card& other)
    : suit(other.GetSuit()), rank(other.GetRank()), ImagePath(other.GetImagePath()) {}

std::string Deck::Card::GetPath() const {
    std::string _ImagePath = "res/textures/cards/";
    switch (rank) {
    case Rank::Ace:
        _ImagePath += "ace";
        break;
    case Rank::Two:
        _ImagePath += "2";
        break;
    case Rank::Three:
        _ImagePath += "3";
        break;
    case Rank::Four:
        _ImagePath += "4";
        break;
    case Rank::Five:
        _ImagePath += "5";
        break;
    case Rank::Six:
        _ImagePath += "6";
        break;
    case Rank::Seven:
        _ImagePath += "7";
        break;
    case Rank::Eight:
        _ImagePath += "8";
        break;
    case Rank::Nine:
        _ImagePath += "9";
        break;
    case Rank::Ten:
        _ImagePath += "10";
        break;
    case Rank::Jack:
        _ImagePath += "jack";
        break;
    case Rank::Queen:
        _ImagePath += "queen";
        break;
    case Rank::King:
        _ImagePath += "king";
        break;
    default:
        std::cout << "Invalid Rank !\n";
        ASSERT(false);
    }

    _ImagePath += "_of_";

    switch (suit) {
    case Suits::Spade:
        _ImagePath += "spades";
        break;
    case Suits::Heart:
        _ImagePath += "hearts";
        break;
    case Suits::Club:
        _ImagePath += "clubs";
        break;
    case Suits::Diamond:
        _ImagePath += "diamonds";
        break;
    default:
        std::cout << "Invalid Suit !\n";
        ASSERT(false);
    }
    _ImagePath += ".png";
    return _ImagePath;
}
