#pragma once
#include <string>
#include <vector>
enum struct Suits { Spade, Heart, Club, Diamond };
enum struct Rank { Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King };

class Deck {
  public:
    struct Card {
        Suits GetSuit() const { return suit; }
        Rank GetRank() const { return rank; }
        const std::string& GetImagePath() const { return ImagePath; }
        Card(const Card& other);
        Card(Suits _suit, Rank _rank) : suit(_suit), rank(_rank), ImagePath(GetPath()) {}

        bool operator==(const Card& other) const { return rank == other.GetRank(); }
        bool operator==(Rank _rank) const { return _rank == rank; }

      private:
        Suits suit;
        Rank rank;
        std::string ImagePath;
        std::string GetPath() const;
    };

    Deck(bool shuffle = false);
    void Shuffle();
    Card Draw();
    Card Peek() const;

  private:
    std::vector<Card> deck;
};
