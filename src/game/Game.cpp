#include "game/Game.hpp"
#include <memory>
Game::Game(GLFWwindow* _window, Renderer& _renderer)
    : GameDeck(std::make_shared<Deck>(true)), PlayerHand(std::make_shared<Hand>(GameDeck)),
      DealerHand(std::make_shared<Hand>(GameDeck)), window(_window),
      CurrentState(GameState::DEALING), renderer(_renderer) {
    InputHandler.SetWindow(window);
}

void Game::UpdateState() {
    switch (CurrentState) {
    case GameState::DEALING:
        UpdateDealing();
        CurrentState = GameState::DEALER_TURN;
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
    }
    case GameState::ROUND_END:
        break;
    }
}

void Game::Render() {

    renderer.DrawSprite(const Sprite& sprite, const glm::mat4& proj, const VertexArray&,
                        const IndexBuffer&, Shader& shader)
}
