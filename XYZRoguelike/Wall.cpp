#include "Wall.h"
#include <ResourceSystem.h>
#include <SpriteColliderComponent.h>
#include <GameWorld.h>
#include <SpriteRendererComponent.h>

namespace XYZRoguelike
{
	Wall::Wall(float x, float y)
	{
		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Wall");
		
		auto wallRenderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
		wallRenderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureShared("platform"));
		wallRenderer->SetPixelSize(32, 32);
		wallRenderer->SetColor(sf::Color::Red); // Красные стены

		auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
		transform->SetLocalPosition(x, y);

		auto body = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
		body->SetKinematic(true); // Стены должны быть кинематичными

		auto collider = gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();
	}

	XYZEngine::GameObject* Wall::GetGameObject()
	{
		return gameObject;
	}
}
