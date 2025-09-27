#include "Floor.h"
#include <ResourceSystem.h>
#include <GameWorld.h>
#include <SpriteRendererComponent.h>

namespace XYZRoguelike
{
	Floor::Floor(float x, float y)
	{
		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Floor");
		
		auto floorRenderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
		if (floorRenderer)
		{
			floorRenderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureShared("platform"));
			floorRenderer->SetPixelSize(32, 32);
			floorRenderer->SetColor(sf::Color(128, 128, 128)); // Серый пол
		}

		auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
		if (transform)
		{
			transform->SetLocalPosition(x, y);
		}

		// Пол не нуждается в физике - это просто декоративный элемент
	}

	XYZEngine::GameObject* Floor::GetGameObject()
	{
		return gameObject;
	}
}
