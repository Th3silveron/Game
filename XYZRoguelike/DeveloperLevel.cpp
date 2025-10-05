#include "DeveloperLevel.h"
#include <ResourceSystem.h>
#include <PlayerControllerComponent.h>
#include <Logger.h>

using namespace XYZEngine;

namespace XYZRoguelike
{
	void DeveloperLevel::Start()
	{
		LOG_INFO("Starting DeveloperLevel initialization...", "LEVEL");
		
		// Загружаем ресурсы
		LOG_INFO("Loading game textures...", "LEVEL");
		ResourceSystem::Instance()->LoadTexture("ball", "Resources/Textures/ball.png");
		ResourceSystem::Instance()->LoadTexture("platform", "Resources/Textures/platform.png");
		
		// Загружаем текстуры тайлов
		LOG_INFO("Loading tile textures...", "LEVEL");
		ResourceSystem::Instance()->LoadTexture("grass", "Resources/Textures/floor/grass.png");
		ResourceSystem::Instance()->LoadTexture("dirt", "Resources/Textures/floor/dirt.png");
		ResourceSystem::Instance()->LoadTexture("stone", "Resources/Textures/floor/stone.png");
		ResourceSystem::Instance()->LoadTexture("path", "Resources/Textures/floor/path.png");
		ResourceSystem::Instance()->LoadTexture("swamp", "Resources/Textures/floor/swamp.png");
		
		// Пытаемся загрузить текстуру грязи, если не найдена - используем dirt
		ResourceSystem::Instance()->LoadTexture("mud", "Resources/Textures/floor/mud.png");
		if (!ResourceSystem::Instance()->GetTextureShared("mud"))
		{
			LOG_WARNING("Mud texture not found, using dirt texture instead", "LEVEL");
			ResourceSystem::Instance()->LoadTexture("mud", "Resources/Textures/floor/dirt.png");
		}
		
		ResourceSystem::Instance()->LoadTexture("tree", "Resources/Textures/decorations/tree.png");
		
		// Загружаем музыку
		LOG_INFO("Loading background music...", "LEVEL");
		ResourceSystem::Instance()->LoadMusic("background_music", "Resources/Sounds/background_music.ogg");
		backgroundMusic = ResourceSystem::Instance()->GetMusic("background_music");
		
		// Загружаем звуки
		LOG_INFO("Loading sound effects...", "LEVEL");
		ResourceSystem::Instance()->LoadSoundBuffer("AppleEat", "Resources/Sounds/AppleEat.wav");
		ResourceSystem::Instance()->LoadSoundBuffer("Death", "Resources/Sounds/Death.wav");

		// Создаем генератор уровня и генерируем уровень (сначала фон)
		LOG_INFO("Generating level (40x40 tiles, 128px each)", "LEVEL");
		levelGenerator = std::make_unique<LevelGenerator>();
		levelGenerator->GenerateLevel(40, 128.0f); // 40x40 тайлов по 128 пикселей = 5120x5120 пикселей
		
		// Создаем игрока
		LOG_INFO("Creating player...", "LEVEL");
		player = std::make_shared<Player>();
		ASSERT_LOG(player != nullptr, "Failed to create player", "LEVEL");
		
		// Создаем врага
		LOG_INFO("Creating enemy...", "LEVEL");
		enemy = std::make_shared<Enemy>();
		ASSERT_LOG(enemy != nullptr, "Failed to create enemy", "LEVEL");
		enemy->SetPlayer(player->GetGameObject());
		
		// Настраиваем систему боя - передаем врагов игроку
		LOG_INFO("Setting up combat system...", "LEVEL");
		std::vector<GameObject*> enemies;
		enemies.push_back(enemy->GetGameObject());
		auto playerController = player->GetGameObject()->GetComponent<PlayerControllerComponent>();
		if (playerController)
		{
			playerController->SetEnemies(enemies);
			LOG_INFO("Player controller configured with enemies", "LEVEL");
		}
		else
		{
			LOG_ERROR("Failed to get PlayerControllerComponent from player", "LEVEL");
		}
		
		// Запускаем фоновую музыку
		if (backgroundMusic)
		{
			LOG_INFO("Starting background music", "LEVEL");
			backgroundMusic->setLoop(true);
			backgroundMusic->setVolume(50.0f);
			backgroundMusic->play();
		}
		else
		{
			LOG_WARNING("Background music not loaded", "LEVEL");
		}
		
		LOG_INFO("DeveloperLevel initialization completed successfully", "LEVEL");
	}
	
	void DeveloperLevel::Restart()
	{
		Stop();
		Start();
	}
	
	void DeveloperLevel::Stop() 
	{
		// Останавливаем музыку
		if (backgroundMusic)
		{
			backgroundMusic->stop();
		}
		
		// Очищаем все объекты
		GameWorld::Instance()->Clear();
		player.reset();
		enemy.reset();
		levelGenerator.reset();
	}
}