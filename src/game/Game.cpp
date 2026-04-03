#include "game/Game.hpp"
#include "core/ResourceManager.hpp"
#include "game/Deck.hpp"
#include "glm/detail/qualifier.hpp"
#include "renderer/Sprite.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

constexpr int WINDOW_WIDTH = 960;
constexpr int WINDOW_HEIGHT = 540;
constexpr bool DEBUG = true;
constexpr int CARD_PADDING_X = 30;
constexpr int PLAYER_CARD_PADDING_Y = 100;
constexpr int DEALER_CARD_PADDING_Y = 350;

Game::Game(Renderer& r, ResourceManager& rm)
    : GameDeck(std::make_shared<Deck>(true)), // PlayerHand(std::make_shared<Hand>(GameDeck)),
      DealerHand(std::make_shared<Hand>(GameDeck)), CurrentState(GameState::DEALING), buttons(),
      PlayerCards(), DealerCards(), renderer(r), resourceManager(rm) {

    Sprite hitSprite = {glm::vec2{100, 100}, glm::vec2{100, 72},
                        resourceManager.Get("res/textures/ui/hit_unpressed.png"), true};
    Sprite hitPressedSprite = {glm::vec2{100, 100}, glm::vec2{150, 75},
                               resourceManager.Get("res/textures/ui/hit_pressed.png"), true};

    Sprite null1 = {glm::vec2{339.5f, 136.5f}, glm::vec2{143, 74},
                    resourceManager.Get("res/textures/ui/null"), true};

    Sprite null2 = {glm::vec2{580.5f, 136.5f}, glm::vec2{142, 74},
                    resourceManager.Get("res/textures/ui/null"), true};
    Sprite null3 = {glm::vec2{454.5, 109.5}, glm::vec2{143, 74},
                    resourceManager.Get("res/textures/ui/null")};
    Sprite playPressedSprite = {glm::vec2{339.5f, 136.5f}, glm::vec2{142, 74},
                                resourceManager.Get("res/textures/play_pressed.png"),
                                true}; // technically the size i set here is incorrecnt.
    // the actual real size is 143 x 73, but the size of the quit button is 142 x 74.
    // Dont pick ms paint as a tool for making assets is the moral of the story here.

    Sprite quitPressedSprite = {glm::vec2{580.5f, 136.5f}, glm::vec2{142, 74},
                                resourceManager.Get("res/textures/ui/quit_pressed.png"), true};

    Button quit = {quitPressedSprite, null2, Action::QUIT};

    Button play = {playPressedSprite, null1, Action::PLAY};

    Button hit = {hitPressedSprite, hitSprite, Action::HIT};

    Sprite standSprite = {glm::vec2{860, 100}, glm::vec2{100, 72},
                          resourceManager.Get("res/textures/ui/stand_unpressed.png")};
    Sprite standPressedSprite = {glm::vec2{860, 100}, glm::vec2{150, 75},
                                 resourceManager.Get("res/textures/ui/stand_pressed.png")};
    Button stand = {standPressedSprite, standSprite, Action::STAND};

    Button continueButton = {null3, null3, Action::CONTINUE};

    buttons[GameState::DEALING].push_back(quit);
    buttons[GameState::DEALING].push_back(play);
    buttons[GameState::PLAYER_TURN].push_back(hit);
    buttons[GameState::PLAYER_TURN].push_back(stand);
    buttons[GameState::ROUND_END].push_back(continueButton);

    table = {glm::vec2{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, glm::vec2{WINDOW_WIDTH, WINDOW_HEIGHT},
             resourceManager.Get("res/textures/ui/table.png")};
    MainMenu = {glm::vec2{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, glm::vec2{857, 431},
                resourceManager.Get("res/textures/ui/main_screen.png")};
    ResultsTable = {glm::vec2{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, glm::vec2{857, 431},
                    resourceManager.Get("res/textures/ui/results_screen.png")};

    std::shared_ptr<Deck::Card> c1 = std::make_shared<Deck::Card>(Suits::Spade, Rank::Ace);

    std::shared_ptr<Deck::Card> c2 = std::make_shared<Deck::Card>(Suits::Spade, Rank::King);

    PlayerHand = std::make_shared<Hand>(GameDeck);

    PlayerCards.push_back({glm::vec2{0, 0}, glm::vec2{CARD_WIDTH, CARD_HEIGHT},
                           resourceManager.Get(PlayerHand->GetHand()->front().GetImagePath())});
    PlayerCards.push_back({glm::vec2{0, 0}, glm::vec2{CARD_WIDTH, CARD_HEIGHT},
                           resourceManager.Get(PlayerHand->GetHand()->back().GetImagePath())});

    DealerCards.push_back({glm::vec2{0, 0}, glm::vec2{CARD_WIDTH, CARD_HEIGHT},
                           resourceManager.Get(DealerHand->GetHand()->front().GetImagePath())});
    DealerCards.push_back({glm::vec2{0, 0}, glm::vec2{CARD_WIDTH, CARD_HEIGHT},
                           resourceManager.Get(DealerHand->GetHand()->back().GetImagePath())});

    BackCard = {glm::vec2{0, 0}, glm::vec2{CARD_WIDTH, CARD_HEIGHT},
                resourceManager.Get("res/textures/cards/back_card.png")};
}

void Game::UpdateState() {
    switch (CurrentState) {
        case GameState::DEALING: {
            Action act = UpdateDealing();
            if (act == Action::WAIT)
                break;
            if (act == Action::PLAY) {
                CurrentState = GameState::PLAYER_TURN;
                break;
            } else
                glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        }
        case GameState::PLAYER_TURN: {
            Action state = UpdatePlayerTurn();

            if (!ActionCounter &&
                PlayerBust) { // this is freaky logic, PlayerBust is only used here.
                PlayerBust = false;
                CurrentState = GameState::ROUND_END;
                break;
            }

            if (PlayerBust) {
                ActionCounter--;
                break;
            }

            if (state == Action::HIT) {
                PlayerHit();
            } else if (state == Action::STAND) {

                if (!ActionCounter && PlayerBlackjack) {
                    PlayerBlackjack = false;
                    CurrentState = GameState::ROUND_END;
                    break;
                }
                if (PlayerBlackjack) {
                    ActionCounter--;
                    break;
                }

                if (PlayerHand->IsBlackJack()) {
                    ActionCounter = ActionLength;
                    PlayerBlackjack = true;
                    break;
                }
                CurrentState = GameState::DEALER_TURN;
                ActionCounter = ActionLength;
            } else if (state == Action::BUST) {
                PlayerBust = true;
                ActionCounter = ActionLength;
            }

            break;
        }

        case GameState::DEALER_TURN: {
            Action state = UpdateDealerTurn();
            if (state == Action::HIT) {
                DealerHit();
            } else if (state == Action::STAND) {
                CurrentState = GameState::ROUND_END;
            } else if (state == Action::BUST)
                CurrentState = GameState::ROUND_END;
            break;
        }
        case GameState::ROUND_END: {
            Action state = UpdateRoundEnd();
            if (state == Action::CONTINUE) {
                CurrentState = GameState::DEALING;
                GameDeck = std::make_shared<Deck>(true);
                PlayerHand = std::make_shared<Hand>(GameDeck);
                DealerHand = std::make_shared<Hand>(GameDeck);

                PlayerCards.clear();
                DealerCards.clear();

                PlayerCards.push_back(
                    {glm::vec2{0, 0}, glm::vec2{CARD_WIDTH, CARD_HEIGHT},
                     resourceManager.Get(PlayerHand->GetHand()->front().GetImagePath())});
                PlayerCards.push_back(
                    {glm::vec2{0, 0}, glm::vec2{CARD_WIDTH, CARD_HEIGHT},
                     resourceManager.Get(PlayerHand->GetHand()->back().GetImagePath())});

                DealerCards.push_back(
                    {glm::vec2{0, 0}, glm::vec2{CARD_WIDTH, CARD_HEIGHT},
                     resourceManager.Get(DealerHand->GetHand()->front().GetImagePath())});
                DealerCards.push_back(
                    {glm::vec2{0, 0}, glm::vec2{CARD_WIDTH, CARD_HEIGHT},
                     resourceManager.Get(DealerHand->GetHand()->back().GetImagePath())});
                ActionCounter = 0;
                PlayerBust = false;
            }
            break;
        }
    }
    for (int a = (int)GameState::DEALING; a <= (int)GameState::ROUND_END; a++) {
        for (auto& b : buttons[(GameState)a]) {
            b.Update();
        }
    }
    InputHandler.ConsumeClick();
}

void Game::PlayerHit() {
    auto c = PlayerHand->Hit();

    PlayerCards.push_back({glm::vec2{0, 0}, glm::vec2{CARD_WIDTH, CARD_HEIGHT},
                           resourceManager.Get(c->GetImagePath())});
}

void Game::DealerHit() {
    auto c = DealerHand->Hit();

    DealerCards.push_back({glm::vec2{0, 0}, glm::vec2{CARD_WIDTH, CARD_HEIGHT},
                           resourceManager.Get(c->GetImagePath())});
}

// I really couldnt be bothered to make animations for the buttons here.
// The thing here that needs to be properly implemented is a Tweening class,
// And I am just unfortunately not knowledgable enough when it comes to game engines.
// The sprites are in the constructor if a future Alfa wants to implement animations.
Action Game::UpdateDealing() {

    Render();
    for (Button& b : buttons[CurrentState]) {
        Action act = b.WasHit();
        if (act == Action::PLAY || act == Action::QUIT)
            return act;
    }
    return Action::WAIT;
}

Action Game::UpdatePlayerTurn() {
    Render();
    if (PlayerHand->IsBlackJack())
        return Action::STAND;
    if (PlayerHand->IsBust()) {
        return Action::BUST;
    }
    for (Button& b : buttons[CurrentState]) {
        Action act = b.WasHit();
        if (act != Action::WAIT) {
            return act;
        }
    }
    return Action::WAIT;
}

Action Game::UpdateDealerTurn() {
    Render();
    if (ActionCounter) {
        ActionCounter--;
        return Action::WAIT;
    }
    ActionCounter = ActionLength;
    if (DealerHand->IsBust())
        return Action::BUST;

    if (DealerHand->GetScore() >= 17 || *DealerHand > *PlayerHand) {
        return Action::STAND;
    }

    return Action::HIT;
}

Action Game::UpdateRoundEnd() {
    Render();
    for (Button& b : buttons[CurrentState]) {
        Action act = b.WasHit();
        if (act != Action::WAIT)
            return act;
    }
    return Action::WAIT;
}

Button::Button(Sprite pressed, Sprite unpressed, Action a)
    : buttonUnpressed(unpressed), action(a), buttonPressed(pressed) {}
const Sprite& Button::GetUnpressed() const { return buttonUnpressed; }
const Sprite& Button::GetPressed() const { return buttonPressed; }

Action Button::WasHit() {
    if (WasHitThisFrame) {
        return action;
    }
    int x = buttonUnpressed.position.x - buttonUnpressed.size.x / 2;
    int y = buttonUnpressed.position.y - buttonUnpressed.size.y / 2;
    int worldY = WINDOW_HEIGHT - buttonUnpressed.position.y - buttonUnpressed.size.y / 2;

    if (input.PeekClicked()) {
        glm::vec2 pos = input.GetPosition();
        if ((pos.x >= x && pos.x <= x + buttonUnpressed.size.x) &&
            (pos.y >= worldY && pos.y <= worldY + buttonUnpressed.size.y)) {
            WasHitThisFrame = true;
            return action;
        }
    }

    return Action::WAIT;
}
void Button::Update() { WasHitThisFrame = false; }
const Action Button::GetAction() const { return action; }

void Game::Render() {

    renderer.DrawSprite(table);
    RenderButtons();
    switch (CurrentState) {

        case GameState::DEALING: {
            renderer.DrawSprite(MainMenu);
            break;
        }

        case GameState::PLAYER_TURN:
            RenderCards(false);
            RenderCards(true);
            RenderScore(false);
            break;
        case GameState::DEALER_TURN:
            RenderCards(false);
            RenderCards(true);
            RenderScore(true);
            break;
        case GameState::ROUND_END:
            RenderCards(false);
            RenderCards(true);
            RenderEnd();
            break;
    }
}

void Game::RenderButtons() {
    for (auto& b : buttons[CurrentState]) {
        if (b.WasHit() != Action::WAIT) {
            b.WaitCount = b.Wait;
        }

        if (b.WaitCount) {
            b.WaitCount--;
            renderer.DrawSprite(b.GetPressed());
            continue;
        }

        renderer.DrawSprite(b.GetUnpressed());
    }
}

void Game::RenderEnd() {

    renderer.DrawSprite(ResultsTable);
    if (PlayerHand->IsBlackJack()) {

        textRenderer.Render("YOUR SCORE: BLACKJACK !!", glm::vec2{200, 300}, 35);

    } else if (!PlayerHand->IsBust()) {
        textRenderer.Render(std::string("YOUR SCORE: ") + std::to_string(PlayerHand->GetScore()),
                            glm::vec2{200, 300}, 35);
    } else {
        textRenderer.Render("YOUR SCORE: BUST !", glm::vec2{200, 300}, 35);
    }
    if (DealerHand->IsBlackJack()) {

        textRenderer.Render("DEALER SCORE: BLACKJACK !!", glm::vec2{200, 250}, 35);
    } else if (!DealerHand->IsBust()) {
        textRenderer.Render(std::string("DEALER SCORE: ") + std::to_string(DealerHand->GetScore()),
                            glm::vec2{200, 250}, 35);
    } else {
        textRenderer.Render("DEALER SCORE: BUST !", glm::vec2{200, 250}, 35);
    }

    if (PlayerHand->IsBust() || *DealerHand > *PlayerHand) {
        textRenderer.Render("YOU LOSE !!", glm::vec2{200, 200}, 35);
    } else if (*PlayerHand > *DealerHand || DealerHand->IsBust()) {
        textRenderer.Render("YOU WON !!", glm::vec2{200, 200}, 35);
    } else if (*PlayerHand == *DealerHand) {
        textRenderer.Render("PUSH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", glm::vec2{200, 200}, 35);
    }
}

void Game::RenderCards(bool dealer = false) {
    if (!dealer) {
        const int nCards = PlayerCards.size();
        const int totalWidth = (CARD_WIDTH * nCards) + (CARD_PADDING_X * (nCards - 1));
        const int edge = (WINDOW_WIDTH - totalWidth) / 2;
        for (int i = 0; i < nCards; ++i) {

            const int card_x = edge + i * (CARD_WIDTH + CARD_PADDING_X);
            PlayerCards[i].position = glm::vec2{card_x, PLAYER_CARD_PADDING_Y};
            renderer.DrawSprite(PlayerCards[i]);
        }
    } else {
        const int nCards = DealerCards.size();
        const int totalWidth = (CARD_WIDTH * nCards) + (CARD_PADDING_X * (nCards - 1));
        const int edge = (WINDOW_WIDTH - totalWidth) / 2;
        if (CurrentState == GameState::PLAYER_TURN) {
            DealerCards[0].position = glm::vec2{edge, DEALER_CARD_PADDING_Y};
            BackCard.position =
                glm::vec2{edge + CARD_WIDTH + CARD_PADDING_X, DEALER_CARD_PADDING_Y};
            renderer.DrawSprite(BackCard);
            renderer.DrawSprite(DealerCards[0]);
            return;
        }

        for (int i = 0; i < nCards; ++i) {
            const int card_x = edge + i * (CARD_WIDTH + CARD_PADDING_X);
            DealerCards[i].position = glm::vec2{card_x, DEALER_CARD_PADDING_Y};
            renderer.DrawSprite(DealerCards[i]);
        }
    }
}

void Game::RenderScore(bool dealer) {
    if (PlayerHand->IsBlackJack()) {

        textRenderer.Render("SCORE: BLACKJACK !!", glm::vec2{420.0f, 50.0f}, 20);

    } else if (!PlayerHand->IsBust())
        textRenderer.Render(std::string("SCORE: ") += std::to_string(PlayerHand->GetScore()),
                            glm::vec2{420.0f, 50.0f}, 20);
    else
        textRenderer.Render("BUST", glm::vec2{440, 50}, 20);

    if (dealer) {
        if (!DealerHand->IsBust())
            textRenderer.Render(std::string("SCORE: ") += std::to_string(DealerHand->GetScore()),
                                glm::vec2{420.0f, WINDOW_HEIGHT - 130.0f}, 20);
        else
            textRenderer.Render("BUST", glm::vec2{440.0f, WINDOW_HEIGHT - 130.0f}, 20);
    }
}
