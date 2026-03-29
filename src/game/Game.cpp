#include "game/Game.hpp"
#include "core/ResourceManager.hpp"
#include "game/Deck.hpp"
#include "renderer/Sprite.hpp"
#include <memory>

constexpr int WINDOW_WIDTH = 960;
constexpr int WINDOW_HEIGHT = 540;
constexpr bool DEBUG = true;
constexpr int CARD_PADDING_X = 30;
constexpr int PLAYER_CARD_PADDING_Y = 100;
constexpr int DEALER_CARD_PADDING_Y = 350;

Game::Game(Renderer& r, ResourceManager& rm)
    : GameDeck(std::make_shared<Deck>(true)), PlayerHand(std::make_shared<Hand>(GameDeck)),
      DealerHand(std::make_shared<Hand>(GameDeck)), CurrentState(GameState::DEALING), buttons(),
      PlayerCards(), DealerCards(), renderer(r), resourceManager(rm) {

    Sprite hitSprite = {glm::vec2{100, 100}, glm::vec2{100, 72},
                        resourceManager.Get("res/textures/ui/hit_unpressed.png")};
    Sprite hitPressedSprite = {glm::vec2{100, 100}, glm::vec2{150, 75},
                               resourceManager.Get("res/textures/ui/hit_pressed.png")};

    Button hit = {hitPressedSprite, hitSprite, Action::HIT};

    Sprite standSprite = {glm::vec2{860, 100}, glm::vec2{100, 72},
                          resourceManager.Get("res/textures/ui/stand_unpressed.png")};
    Sprite standPressedSprite = {glm::vec2{860, 100}, glm::vec2{150, 75},
                                 resourceManager.Get("res/textures/ui/stand_pressed.png")};
    Button stand = {standPressedSprite, standSprite, Action::STAND};
    buttons[GameState::DEALING].push_back(hit);
    buttons[GameState::DEALING].push_back(stand);
    buttons[GameState::PLAYER_TURN].push_back(hit);
    buttons[GameState::PLAYER_TURN].push_back(stand);

    table = {glm::vec2{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, glm::vec2{WINDOW_WIDTH, WINDOW_HEIGHT},
             resourceManager.Get("res/textures/ui/table.png")};
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
        case GameState::DEALING:
            UpdateDealing();
            CurrentState = GameState::PLAYER_TURN;
            break;
        case GameState::PLAYER_TURN: {
            Action state = UpdatePlayerTurn();
            if (state == Action::HIT) {
                PlayerHit();

            } else if (state == Action::STAND) {
                CurrentState = GameState::DEALER_TURN;
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
            }
            break;
        }
        case GameState::ROUND_END:
            Render();
    }
    for (int a = (int)GameState::DEALING; a <= (int)GameState::DEALER_TURN; a++) {
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

Action Game::UpdateDealing() {
    Render();
    return Action::WAIT;
}

Action Game::UpdatePlayerTurn() {
    Render();
    if (PlayerHand->IsBust()) {
        return Action::BUST;
    }
    for (Button b : buttons[CurrentState]) {
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

    if (DealerHand->GetScore() >= 17) {
        return Action::STAND;
    }

    return Action::HIT;
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
        std::cout << "(" << pos.x << ", " << pos.y << ")\n";
        if ((pos.x >= x && pos.x <= x + buttonUnpressed.size.x) &&
            (pos.y >= worldY && pos.y <= worldY + buttonUnpressed.size.y)) {
            std::cout << "my shit pressed\n";
            WasHitThisFrame = true;
            return action;
        }
    }

    return Action::WAIT;
}
void Button::Update() { WasHitThisFrame = false; }

void Game::Render() {
    renderer.DrawSprite(table);
    RenderButtons();
    switch (CurrentState) {

        case GameState::DEALING: {
            RenderCards(false);
            RenderCards(true);
            break;
        }

        case GameState::PLAYER_TURN:
            RenderCards(false);
            RenderCards(true);
            break;
        case GameState::DEALER_TURN:
            RenderCards(false);
            RenderCards(true);
            break;
        case GameState::ROUND_END:
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
