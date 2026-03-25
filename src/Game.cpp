#include "Deck.hpp"
#include "Hand.hpp"
#include <iostream>
#include <memory>
class Game {
    std::shared_ptr<Deck> GameDeck;
    Hand PlayerHand;
    Hand DealerHand;

  public:
    Game() : GameDeck(std::make_shared<Deck>(Deck())), PlayerHand(GameDeck), DealerHand(GameDeck) {
        if (PlayerHand.IsBlackJack() && !DealerHand.IsBlackJack())
            std::cout << "BlackJack ! You win !";

        GameLoop();
    }

    void GameLoop() {}
};

int main() {}
