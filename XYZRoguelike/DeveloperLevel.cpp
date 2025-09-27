#include "DeveloperLevel.h"
#include <ResourceSystem.h>

using namespace XYZEngine;

namespace XYZRoguelike
{
	void DeveloperLevel::Start()
	{
		// Загружаем ресурсы
		ResourceSystem::Instance()->LoadTexture("ball", "Resources/Textures/ball.png");
		ResourceSystem::Instance()->LoadTexture("platform", "Resources/Textures/platform.png");
		
		// Загружаем музыку
		ResourceSystem::Instance()->LoadMusic("background_music", "Resources/Sounds/background_music.ogg");
		backgroundMusic = ResourceSystem::Instance()->GetMusic("background_music");
		
		// Загружаем звуки
		ResourceSystem::Instance()->LoadSoundBuffer("AppleEat", "Resources/Sounds/AppleEat.wav");
		ResourceSystem::Instance()->LoadSoundBuffer("Death", "Resources/Sounds/Death.wav");

		// Создаем генератор уровня и генерируем уровень (сначала фон)
		levelGenerator = std::make_unique<LevelGenerator>();
		levelGenerator->GenerateLevel(40, 32.0f); // 40x40 тайлов по 32 пикселя = 1280x1280 пикселей
		
		// Создаем игрока
		player = std::make_shared<Player>();
		
		// Создаем врага
		enemy = std::make_shared<Enemy>();
		enemy->SetPlayer(player->GetGameObject());
		
		// Запускаем фоновую музыку
		if (backgroundMusic)
		{
			backgroundMusic->setLoop(true);
			backgroundMusic->setVolume(50.0f);
			backgroundMusic->play();
		}
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