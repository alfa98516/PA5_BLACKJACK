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

    Button hit = Button(hitPressedSprite, hitSprite, Action::HIT);
    table = {glm::vec2{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, glm::vec2{WINDOW_WIDTH, WINDOW_HEIGHT},
             resourceManager.Get("res/textures/ui/table.png")};
    buttons[GameState::DEALING].push_back(hit);
    buttons[GameState::DEALING]; // TODO: make support for stand button lowkey
    buttons[GameState::PLAYER_TURN].push_back(hit);

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
                PlayerHand->Hit();

            } else if (state == Action::STAND)
                CurrentState = GameState::DEALER_TURN;
            break;
        }

        case GameState::DEALER_TURN: {
            Action state = UpdateDealerTurn();
            if (state == Action::HIT) {
                DealerHand->Hit();
            } else if (state == Action::STAND) {
                CurrentState = GameState::ROUND_END;
            }
            break;
        }
        case GameState::ROUND_END:
            Render();
    }
}
void Game::UpdateDealing() { Render(); }
Action Game::UpdatePlayerTurn() {
    Render();
    if (PlayerHand->IsBust()) {
        return Action::BUST;
    }
    for (Button b : buttons[CurrentState]) {
        Action act = b.WasHit();
    }
    return Action::WAIT;
}

Button::Button(Sprite pressed, Sprite unpressed, Action a)
    : buttonUnpressed(unpressed), action(a), buttonPressed(pressed) {}
const Sprite& Button::GetUnpressed() const { return buttonUnpressed; }
const Sprite& Button::GetPressed() const { return buttonPressed; }

Action Button::WasHit() {
    int x = buttonUnpressed.position.x;
    int y = buttonUnpressed.position.y;
    int worldY = WINDOW_HEIGHT - y;

    if (input.WasClicked()) {
        glm::vec2 pos = input.GetPosition();
        std::cout << "(" << pos.x << ", " << pos.y << ")\n";
        if ((pos.x >= x && pos.x <= x + buttonUnpressed.size.x) &&
            (pos.y >= worldY && pos.y <= worldY + buttonUnpressed.size.y)) {
            std::cout << "my shit pressed\n";
            return action;
        }
    }

    return Action::WAIT;
}

void Game::Render() {
    renderer.DrawSprite(table);

    RenderButtons();
    switch (CurrentState) {

        case GameState::DEALING: {
            int padding2 = (WINDOW_WIDTH - CARD_PADDING_X - CARD_WIDTH * 2) / 2;

            PlayerCards[0].position = glm::vec2{padding2, PLAYER_CARD_PADDING_Y};
            renderer.DrawSprite(PlayerCards[0]);
            PlayerCards[1].position =
                glm::vec2{padding2 + CARD_WIDTH + CARD_PADDING_X, PLAYER_CARD_PADDING_Y};
            renderer.DrawSprite(PlayerCards[1]);

            DealerCards[0].position = glm::vec2{padding2, DEALER_CARD_PADDING_Y};
            renderer.DrawSprite(DealerCards[0]);

            BackCard.position = {padding2 + CARD_WIDTH + CARD_PADDING_X, DEALER_CARD_PADDING_Y};
            renderer.DrawSprite(BackCard);
        }

        case GameState::PLAYER_TURN:

        case GameState::DEALER_TURN:
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

Action Game::UpdateDealerTurn() { return Action::WAIT; }
