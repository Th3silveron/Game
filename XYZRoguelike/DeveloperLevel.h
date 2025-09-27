#pragma once

#include <iostream>
#include <array>
#include <Scene.h>
#include "Player.h"
#include "Enemy.h"
#include "LevelGenerator.h"
#include <SFML/Audio.hpp>

using namespace XYZEngine;

namespace XYZRoguelike
{
	class DeveloperLevel : public Scene
	{
	public:
		void Start() override;
		void Restart() override;
		void Stop() override;
	private:
		std::shared_ptr<Player> player;
		std::shared_ptr<Enemy> enemy;
		std::unique_ptr<LevelGenerator> levelGenerator;
		sf::Music* backgroundMusic;
	};
}