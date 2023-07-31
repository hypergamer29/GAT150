#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Renderer/ModelManager.h"
#include "Input/InputSystem.h"

#include "Player.h"
#include "Enemy.h"

#include "Renderer/Font.h"
#include "Renderer/Text.h"
#include "Audio/AudioSystem.h"
#include "Framework/Scene.h"
#include "SpaceGame.h"

#include "Renderer/ParticleSystem.h"
#include "Framework/Emitter.h"

#include <iostream>
#include <vector>
#include <thread>


using namespace std;


class Star {
public:
	Star(const Loki::vec2 pos, const Loki::vec2 vel) : 
		m_pos{ pos }, 
		m_vel{ vel } 
	{}
	
	void Update(int width, int height) {
		m_pos += m_vel * Loki::g_time.GetDeltaTime();
		if (m_pos.x >= width) m_pos.x = 0;
		if (m_pos.y >= height) m_pos.y = 0;
	}

	void Draw(Loki::Renderer& renderer) {
		renderer.DrawPoint(m_pos.x, m_pos.y);
	}

public:
	Loki::vec2 m_pos;
	Loki::vec2 m_vel;

};


int main(int argc, char* argv[]) {
	Loki::MemoryTracker::Initialize();

	Loki::seedRandom((unsigned int)time(nullptr));
	Loki::setFilePath("assets");

	Loki::g_renderer.Initialize();
	Loki::g_renderer.CreateWindow("CSC196", 800, 600);

	Loki::g_inputSystem.Initialize();

	Loki::g_audioSystem.Initialize();
	Loki::g_audioSystem.AddAudio("Pew", "Pew.wav");

	Loki::SpaceGame game;
	game.Initialize();



	//stars
	vector<Star> stars;
	for (int i = 0; i < 100; i++) {
		Loki::vec2 pos(Loki::vec2(Loki::randomf((float)Loki::g_renderer.GetWidth()), Loki::randomf((float)Loki::g_renderer.GetHeight())));
		Loki::vec2 vel((float)Loki::randomf(50.0f, 90.f), 0.0f);

		stars.push_back(Star(pos, vel));
	}

	// main game loop
	bool quit = false;
	while (!quit) {

		//update Engine
		Loki::g_time.Tick();
		Loki::g_audioSystem.Update();
		Loki::g_inputSystem.Update();
		if (Loki::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE)) {
			quit = true;
		}

		Loki::g_particleSystem.Update(Loki::g_time.GetDeltaTime());

		//Objects update
		game.Update(Loki::g_time.GetDeltaTime());

		Loki::g_renderer.SetColor(0, 0, 0, 0);
		Loki::g_renderer.BeginFrame();

		// draw the ~stars~ (background)
		for (auto& star : stars) {
			star.Update(Loki::g_renderer.GetWidth(), Loki::g_renderer.GetHeight());
			Loki::g_renderer.SetColor(255, 255, 255, 255);
			Loki::g_renderer.DrawPoint((float)star.m_pos.x, (float)star.m_pos.y);
		}

		//Draw game
		game.Draw(Loki::g_renderer);
		Loki::g_particleSystem.Draw(Loki::g_renderer);

		Loki::g_renderer.EndFrame();
	}
	stars.clear();
	
	return 0;
}