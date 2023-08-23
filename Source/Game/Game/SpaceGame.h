#pragma once
#include "Framework/Game.h"
#include "Framework/Event/EventManager.h"
#include "Renderer/Text.h"

namespace Loki {
    class SpaceGame : public Loki::Game, Loki::IEventListener {
    public:

        enum class eState {
            Title,
            StartGame,
            StartLevel,
            Game,
            PlayerDeadStart,
            PlayerDead,
            GameOverStart,
            GameOver
        };

    public:

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float dt) override;
        virtual void Draw(Loki::Renderer& renderer) override;

        void SetState(eState state) { m_state = state; }
        void OnAddPoints(const Loki::Event& event);
        void OnPlayerDead(const Loki::Event& event);

    private:
        eState m_state = eState::Title;
        float m_spawnTimer = 0;
        float m_spawnTime = 2.0f;

        float m_stateTimer = 0;
        float m_gameTimer = 0;

        std::shared_ptr<Loki::Font> m_font;
        std::unique_ptr<Loki::Text> m_scoreText;
        std::unique_ptr<Loki::Text> m_healthText;
        std::unique_ptr<Loki::Text> m_titleText;
        std::unique_ptr<Loki::Text> m_timerText;
        std::unique_ptr<Loki::Text> m_gameOverText;

    };
}