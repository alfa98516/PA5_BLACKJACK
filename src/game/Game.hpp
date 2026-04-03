#pragma once
#include "core/Input.hpp"
#include "core/ResourceManager.hpp"
#include "game/Deck.hpp"
#include "game/Hand.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/TextRenderer.hpp"
#include <memory>
#include <sys/types.h>
#include <unordered_map>

enum class Action {
    PLAY,
    QUIT,
    WAIT,
    HIT,
    STAND,
    BUST,
    CONTINUE

};

enum class GameState {
    DEALING = 0,
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
    bool WasHitThisFrame = false;

  public:
    static constexpr int Wait = 80;
    Button(Sprite, Sprite, Action);
    Action WasHit();
    void Update();
    const Sprite& GetUnpressed() const;
    const Sprite& GetPressed() const;
    const Action GetAction() const;
    int WaitCount = 0; // The idea behind the waits is that i wait for x amount of frames,
                       // the waitcount is intended to tell the renderer:
                       // "don't render me as unpressed until im zero,
                       // decrement me instead and render the pressed sprite"
                       // This is once again broken if you're using a Nvidia card,
                       // I can't turn on vsync for Nvidia cards
};

class Game {
    GLFWwindow* window;

    Input& InputHandler = Input::Instance();
    ResourceManager& resourceManager;
    Renderer& renderer;
    TextRenderer& textRenderer = TextRenderer::Instance();

    std::unordered_map<GameState, std::vector<Button> > buttons;

    std::shared_ptr<Deck> GameDeck;
    std::shared_ptr<Hand> PlayerHand;
    std::shared_ptr<Hand> DealerHand;

    const int ActionLength = 400;
    int ActionCounter = 0; // much like the counter on the buttons, designed to add waiting time.

    bool PlayerBust = false;
    bool PlayerBlackjack = false;

    std::vector<Sprite> PlayerCards;
    std::vector<Sprite> DealerCards;

    Sprite table;
    Sprite BackCard;
    Sprite MainMenu;
    Sprite ResultsTable;

    GameState CurrentState;
    Action UpdateDealing();
    Action UpdatePlayerTurn();
    void PlayerHit();
    void DealerHit();
    Action UpdateDealerTurn();
    Action UpdateRoundEnd();

    void Render();
    void RenderButtons();
    void RenderCards(bool);
    void RenderScore(bool);
    void RenderEnd();

  public:
    Game(Renderer&, ResourceManager&);
    void UpdateState();
};
