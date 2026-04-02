#include "Deck.hpp"
#include "renderer/Sprite.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <random>

std::random_device rd;

Deck::Deck(bool shuffle) : deck() {
    for (int _rank = 1; _rank < 14; ++_rank) {
        for (int _suit = 0; _suit < 4; ++_suit) {
            deck.push_back(std::make_shared<Card>((Suits)_suit, (Rank)_rank));
        }
    }
    if (shuffle) {
        Shuffle();
    }
}

std::shared_ptr<Deck::Card> Deck::Draw() {
    std::shared_ptr<Deck::Card> card = deck.back();
    deck.pop_back();
    return card;
}

void Deck::Shuffle() { std::shuffle(deck.begin(), deck.end(), std::mt19937(rd())); }

std::shared_ptr<Deck::Card> Deck::Peek() const { return deck.back(); }

Deck::Card::Card(const Card& other)
    : suit(other.GetSuit()), rank(other.GetRank()), ImagePath(other.GetImagePath()) {}

void Deck::Card::PrintCard() const {
    switch (rank) {

        case Rank::Ace:
            std::cout << "Ace of ";
            break;
        case Rank::Two:
        case Rank::Three:
        case Rank::Four:
        case Rank::Five:
        case Rank::Six:
        case Rank::Seven:
        case Rank::Eight:
        case Rank::Nine:
        case Rank::Ten:
            std::cout << (int)rank << " of ";
            break;
        case Rank::Jack:
            std::cout << "Jack of ";
            break;
        case Rank::Queen:
            std::cout << "Queen of ";
            break;
        case Rank::King:
            std::cout << "King of ";
            break;
    }

    switch (suit) {

        case Suits::Spade:
            std::cout << "Spades\n";
            break;
        case Suits::Heart:
            std::cout << "Hearts\n";
            break;
        case Suits::Club:
            std::cout << "Clubs\n";
            break;
        case Suits::Diamond:
            std::cout << "Diamonds\n";
            break;
    }
}

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
            assert(false);
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
            assert(false);
    }
    _ImagePath += ".png";
    return _ImagePath;
}
