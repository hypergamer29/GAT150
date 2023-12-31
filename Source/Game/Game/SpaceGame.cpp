#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Framework/Framework.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"


namespace Loki {
	bool SpaceGame::Initialize() {
		m_scene = std::make_unique<Loki::Scene>();
		m_scene->Load("Scene.json");
		m_scene->Initialize();

		// create font / text objects
		//m_font = Loki::g_resources.Get<Loki::Font>("arcade.TTF", 24);
		m_scoreText = std::make_unique<Loki::Text>(GET_RESOURCE(Loki::Font, "arcade.TTF", 24));
		m_scoreText->Create(Loki::g_renderer, "SCORE: " + std::to_string(GetScore()), Loki::Color{1, 1, 1, 1});

		m_font = std::make_shared<Loki::Font>("arcade.TTF", 24);
		m_healthText = std::make_unique<Loki::Text>(GET_RESOURCE(Loki::Font, "arcade.TTF", 24));

		m_timerText = std::make_unique<Loki::Text>(GET_RESOURCE(Loki::Font, "arcade.TTF", 24));

		m_font = std::make_shared<Loki::Font>("arcade.TTF", 48);
		m_titleText = std::make_unique<Loki::Text>(GET_RESOURCE(Loki::Font, "arcade.TTF", 24));
		m_titleText->Create(Loki::g_renderer, "Average Space Shooter", Loki::Color{1, 1, 1, 1});

		m_gameOverText = std::make_unique<Loki::Text>(GET_RESOURCE(Loki::Font, "arcade.TTF", 24));
		m_gameOverText->Create(Loki::g_renderer, "GAME OVER", Loki::Color{1, 0, 0, 1});

		// add Events
		EVENT_SUBSCRIBE("OnAddPoints", SpaceGame::OnAddPoints)
		EVENT_SUBSCRIBE("OnPlayerDead", SpaceGame::OnPlayerDead)

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
				//m_scene->GetActorByName("Background")->active = false;
			}
		break;

		case SpaceGame::eState::StartGame:
			m_score = 0;
			m_lives = 3;
			m_gameTimer = 0;
			m_state = eState::StartLevel;
		break;

		case SpaceGame::eState::StartLevel: {
			m_scene->RemoveAll();
			m_gameTimer = dt;

			// create player
			auto player = std::make_unique<Player>(20.0f, Loki::Pi, Loki::Transform{ {400, 300}, 0, 3 });
			player->tag = "Player";
			player->m_game = this;

			// create components
			auto renderComponent = Loki::Factory::Instance().Create<SpriteComponent>("SpriteComponent"); //std::make_unique<Loki::SpriteComponent>();
			renderComponent->m_texture = GET_RESOURCE(Loki::Texture, "ship.png", Loki::g_renderer);
			player->AddComponent(std::move(renderComponent));

			// // txt ship render
			//auto renderComponent = std::make_unique<Loki::ModelRenderComponent>();
			//renderComponent->m_model = Loki::g_resources.Get<Loki::Model>("star.txt", Loki::g_renderer);
			//player->AddComponent(std::move(renderComponent));

			auto physicsComponent = std::make_unique<Loki::EnginePhysicsComponent>();
			physicsComponent->m_damping = 0.9f;
			player->AddComponent(std::move(physicsComponent));

			auto collisionComponent = std::make_unique<Loki::CircleCollisionComponent>();
			collisionComponent->m_radius = 30.0f;
			player->AddComponent(std::move(collisionComponent));

			player->Initialize();
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
					enemy = std::make_unique<Enemy>(Loki::randomf(175.0f, 250.0f), Loki::Pi / 3.0f, Loki::Transform{ {Loki::randomf(800), Loki::randomf(600)}, Loki::randomf(Loki::twoPi), 2});
				}
				else {
					enemy = std::make_unique<Enemy>(Loki::randomf(75.0f, 150.0f), Loki::Pi, Loki::Transform{ {Loki::randomf(800), Loki::randomf(600)}, Loki::randomf(Loki::twoPi), 2});
				}
				//make enemy
				enemy->tag = "Enemy";
				enemy->m_game = this;

				//create components
				auto renderComponent = std::make_unique<Loki::SpriteComponent>();
				renderComponent->m_texture = GET_RESOURCE(Loki::Texture, "Enemy.png", Loki::g_renderer);
				enemy->AddComponent(std::move(renderComponent));

				auto collisionComponent = std::make_unique<Loki::CircleCollisionComponent>();
				collisionComponent->m_radius = 30.0f;
				enemy->AddComponent(std::move(collisionComponent));

				enemy->Initialize();
				m_scene->Add(std::move(enemy));

				//create components
				/*std::unique_ptr<Loki::SpriteComponent> component = std::make_unique<Loki::SpriteComponent>();
				component->m_texture = GET_RESOURCE(Loki::Texture, "ship.png", Loki::g_renderer);
				enemy->AddComponent(std::move(component));
				*/

			}
			m_timerText->Create(Loki::g_renderer, "Timer: " + std::to_string((int)m_gameTimer), Loki::Color{1, 1, 1, 1});
			m_healthText->Create(Loki::g_renderer, "HEALTH: " + std::to_string(Health), Loki::Color{1, 0, 0, 1});
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

	void SpaceGame::OnAddPoints(const Loki::Event& event) {
		m_score += std::get<int>(event.data);
	}

	void SpaceGame::OnPlayerDead(const Loki::Event& event) {
		m_lives--;
		m_state = eState::PlayerDeadStart;
	}
}