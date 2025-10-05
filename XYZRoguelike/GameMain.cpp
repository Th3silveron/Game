// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include "Player.h"
#include <Engine.h>
#include <ResourceSystem.h>
#include "DeveloperLevel.h"
#include <Matrix2D.h>

using namespace XYZRoguelike;

int main()
{
	XYZEngine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1920, 1080), "XYZRoguelike"));

	auto developerLevel = std::make_shared<DeveloperLevel>();
	developerLevel->Start();

	XYZEngine::Engine::Instance()->Run();

	return 0;
}
