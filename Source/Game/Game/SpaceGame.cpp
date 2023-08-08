#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Framework/Actor.h"

#include "Framework/Emitter.h"
#include "Renderer/Renderer.h"
#include "Renderer/ParticleSystem.h"

#include "Framework/Scene.h"
#include "Framework/Game.h"
#include "Framework/Resource/RecourceManager.h"
#include "Framework/Components/SpriteComponent.h"
#include "Framework/Components/EnginePhysicsComponent.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Renderer/Text.h"
#include "Renderer/ModelManager.h"

namespace Loki {
	bool SpaceGame::Initialize() {
		m_scene = std::make_unique<Loki::Scene>();

		// create font / text objects
		//m_font = Loki::g_resources.Get<Loki::Font>("arcade.TTF", 24);
		m_scoreText = std::make_unique<Loki::Text>(Loki::g_resources.Get<Loki::Font>("arcade.TTF", 24));
		m_scoreText->Create(Loki::g_renderer, "SCORE: " + std::to_string(GetScore()), Loki::Color{1, 1, 1, 1});

		m_font = std::make_shared<Loki::Font>("arcade.TTF", 24);
		m_healthText = std::make_unique<Loki::Text>(Loki::g_resources.Get<Loki::Font>("arcade.TTF", 24));
		m_healthText->Create(Loki::g_renderer, "HEALTH: " , Loki::Color{1, 0, 0, 1});

		m_timerText = std::make_unique<Loki::Text>(Loki::g_resources.Get<Loki::Font>("arcade.TTF", 24));
		m_timerText->Create(Loki::g_renderer, "TIME: ", Loki::Color{1, 1, 1, 1});

		m_font = std::make_shared<Loki::Font>("arcade.TTF", 48);
		m_titleText = std::make_unique<Loki::Text>(Loki::g_resources.Get<Loki::Font>("arcade.TTF", 24));
		m_titleText->Create(Loki::g_renderer, "Average Space Shooter", Loki::Color{1, 1, 1, 1});

		m_gameOverText = std::make_unique<Loki::Text>(Loki::g_resources.Get<Loki::Font>("arcade.TTF", 24));
		m_gameOverText->Create(Loki::g_renderer, "GAME OVER", Loki::Color{1, 0, 0, 1});

		// load audio
		Loki::g_audioSystem.AddAudio("Pew", "Pew.wav");
		//Loki::g_audioSystem.AddAudio("destroyed", "destroyed.wav");
		//Loki::g_audioSystem.AddAudio("enemyDestroyed", "enemy_destroyed.wav");


		//Loki::g_audioSystem.PlayOneShot("music", true);

		return true;
	}

	void SpaceGame::Shutdown() {
		//
	}

	void SpaceGame::Update(float dt)
	{
		switch (m_state) {
		case SpaceGame::eState::Title:
			if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) {
				m_state = eState::StartGame;
			}
			break;

		case SpaceGame::eState::StartGame:
			m_score = 0;
			m_lives = 3;
			m_score = 0;
			m_gameTimer = 0;
			m_state = eState::StartLevel;
			break;

		case SpaceGame::eState::StartLevel: {
			m_scene->RemoveAll();
			m_gameTimer = dt;

			// creat player
			auto player = std::make_unique<Player>(20.0f, Loki::Pi, Loki::Transform{ {400, 300}, 0, 4 });
			player->m_tag = "Player";
			player->m_game = this;

			//create components
			auto renderComponent = std::make_unique<Loki::SpriteComponent>();
			renderComponent->m_texture = Loki::g_resources.Get<Loki::Texture>("ship.png", Loki::g_renderer);
			player->AddComponent(std::move(renderComponent));

			auto physicsComponent = std::make_unique<Loki::EnginePhysicsComponent>();
			physicsComponent->m_damping = 0.9f;
			player->AddComponent(std::move(physicsComponent));

			m_scene->Add(std::move(player));
		}
			m_state = eState::Game;
			break;

		case SpaceGame::eState::Game: {
			m_gameTimer += dt;
			m_spawnTimer += dt;

			int Health = m_scene->GetActor<Player>()->GetLifeForce();

			std::unique_ptr<Enemy> enemy;
			if (m_spawnTimer >= m_spawnTime) {
				m_spawnTimer = 0;
				float n = Loki::randomf(0, 1.0f);
				if (n <= 0.15) {
					enemy = std::make_unique<Enemy>(Loki::randomf(175.0f, 250.0f), Loki::Pi / 3.0f, Loki::Transform{ {Loki::randomf(800), Loki::randomf(600)}, Loki::randomf(Loki::twoPi), 3});
				}
				else {
					enemy = std::make_unique<Enemy>(Loki::randomf(75.0f, 150.0f), Loki::Pi, Loki::Transform{ {Loki::randomf(800), Loki::randomf(600)}, Loki::randomf(Loki::twoPi), 3});
				}
				//make enemy
				enemy->m_tag = "Enemy";
				enemy->m_game = this;
				m_scene->Add(std::move(enemy));

				//create components
				/*std::unique_ptr<Loki::SpriteComponent> component = std::make_unique<Loki::SpriteComponent>();
				component->m_texture = Loki::g_resources.Get<Loki::Texture>("ship.png", Loki::g_renderer);
				enemy->AddComponent(std::move(component));
				*/

			}
			m_scoreText->Create(Loki::g_renderer, "Score: " + std::to_string(GetScore()), Loki::Color{1, 1, 1, 1});
			m_timerText->Create(Loki::g_renderer, "Timer: " + std::to_string((int)m_gameTimer), Loki::Color{1, 1, 1, 1});
			m_healthText->Create(Loki::g_renderer, "HEALTH: " + std::to_string(Health), Loki::Color{1, 1, 1, 1});
		}

			break;

		case SpaceGame::eState::PlayerDeadStart:
			m_stateTimer = 3;
			if (m_lives == 0) m_state = eState::GameOver;
			else m_state = eState::PlayerDead;
			break;

		case SpaceGame::eState::PlayerDead:
			m_stateTimer -= dt;
			if (m_stateTimer <= 0) {
				m_state = eState::StartLevel;
			}
			break;

		case SpaceGame::eState::GameOver:
			m_stateTimer -= dt;
			if (m_stateTimer <= 0) {
				m_state = eState::Title;
			}
			break;
		default:
			break;
		}

		//SPARKELSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
		/*Loki::EmitterData data;
		data.burst = true;
		data.burstCount = 100;
		data.spawnRate = 200;
		data.angle = 0;
		data.angleRange = Loki::Pi;
		data.lifetimeMin = 0.5f;
		data.lifetimeMax = 1.5f;
		data.speedMin = 50;
		data.speedMax = 250;
		data.damping = 0.5f;
		data.color = Loki::Color{ 1, 0, 0, 1 };

		Loki::Transform transform{ { Loki::g_inputSystem.GetMousePosition() }, 0, 1 };
		auto emitter = std::make_unique<Loki::Emitter>(transform, data);

		emitter->m_lifespan = 1.0f;
		m_scene->Add(std::move(emitter));*/

		m_scene->Update(dt);
	}

	void SpaceGame::Draw(Loki::Renderer& renderer) {
		if (m_state == eState::Title) {
			m_titleText->Draw(renderer, 200, 300);
		}

		if (m_state == eState::GameOver) {
			m_gameOverText->Draw(renderer, 250, 300);
		}

		if (m_state != eState::Title) {
			m_scoreText->Draw(renderer, 40, 40);
			m_timerText->Draw(renderer, 40, 540);
			m_healthText->Draw(renderer, 40, 300);
		}

		m_scene->Draw(renderer);
	}
}