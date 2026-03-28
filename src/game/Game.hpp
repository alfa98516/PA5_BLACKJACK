#pragma once
#include "core/Input.hpp"
#include "core/ResourceManager.hpp"
#include "game/Deck.hpp"
#include "game/Hand.hpp"
#include "renderer/Renderer.hpp"
#include <memory>
#include <unordered_map>

enum class Action {
    WAIT,
    HIT,
    STAND,
    BUST

};

enum class GameState {
    DEALING,
    PLAYER_TURN,
    DEALER_TURN,
    ROUND_END

};

struct Button {
  private:
    Input& input = Input::Instance();
    Sprite buttonPressed;
    Sprite buttonUnpressed;
    Action action;

  public:
    static constexpr int Wait = 100;
    Button(Sprite, Sprite, Action);
    Action WasHit();
    const Sprite& GetUnpressed() const;
    const Sprite& GetPressed() const;
    int WaitCount = 0; // The idea behind the waits is that i wait for x amount of frames,
                       // the waitcount is intended to tell the renderer:
                       // "don't render me as unpressed until im zero,
                       // decrement me instead and render the pressed sprite"
};

class Game {
    GLFWwindow* window;
    Input& InputHandler = Input::Instance();
    ResourceManager& resourceManager;
    Renderer& renderer;
    std::unordered_map<GameState, std::vector<Button> > buttons;
    std::shared_ptr<Deck> GameDeck;
    std::shared_ptr<Hand> PlayerHand;
    std::shared_ptr<Hand> DealerHand;
    std::vector<Sprite> PlayerCards;
    std::vector<Sprite> DealerCards;
    Sprite table;
    Sprite BackCard;
    GameState CurrentState;
    void UpdateDealing();
    Action UpdatePlayerTurn();
    Action UpdateDealerTurn();
    void UpdateRoundEnd();
    void Render();
    void RenderButtons();

  public:
    Game(Renderer&, ResourceManager&);
    void UpdateState();
};
