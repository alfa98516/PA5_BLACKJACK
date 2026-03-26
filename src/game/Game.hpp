#pragma once
#include "core/Input.hpp"
#include "core/ResourceManager.hpp"
#include "game/Deck.hpp"
#include "game/Hand.hpp"
#include "renderer/Renderer.hpp"
#include <memory>

enum class Action {
    WAIT,
    HIT,
    STAND

};
enum class GameState {
    DEALING,
    PLAYER_TURN,
    DEALER_TURN,
    ROUND_END

};
class Game {
    GLFWwindow* window;
    Input& InputHandler = Input::Instance();
    ResourceManager& resourceManager = ResourceManager::Instance();
    Renderer& renderer;
    std::shared_ptr<Deck> GameDeck;
    std::shared_ptr<Hand> PlayerHand;
    std::shared_ptr<Hand> DealerHand;
    GameState CurrentState;

  public:
    Game(GLFWwindow*, Renderer&);
    void UpdateState();
    void UpdateDealing();
    Action UpdatePlayerTurn();
    Action UpdateDealerTurn();
    void UpdateRoundEnd();
    void Render();
};
