#include "Player.h"
#include <ResourceSystem.h>
#include <SpriteColliderComponent.h>
#include <PlayerControllerComponent.h>

namespace XYZRoguelike
{
	Player::Player()
	{
		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Player");
		auto playerRenderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();

		playerRenderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(32, 32);
		playerRenderer->SetColor(sf::Color::Blue); // Синий цвет для игрока

		auto playerInput = gameObject->AddComponent<XYZEngine::InputComponent>();

		auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
		transform->SetLocalPosition(640.0f, 640.0f); // Центр уровня (40*32/2 = 640)

		auto playerCamera = gameObject->AddComponent<XYZEngine::CameraComponent>();
		playerCamera->SetWindow(&XYZEngine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1280, 720);

		auto body = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
		body->SetLinearDamping(5.0f); // Добавляем демпфирование для плавного движения

		auto collider = gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

		auto controller = gameObject->AddComponent<XYZEngine::PlayerControllerComponent>();
		controller->SetMovementSpeed(1.7f); // Уменьшили в 4 раза: 6.7f / 4 ≈ 1.7f
	}

	XYZEngine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}
}