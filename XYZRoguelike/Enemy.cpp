#include "Enemy.h"
#include <ResourceSystem.h>
#include <SpriteColliderComponent.h>
#include <EnemyAIComponent.h>
#include <GameWorld.h>
#include <SpriteRendererComponent.h>

namespace XYZRoguelike
{
	Enemy::Enemy()
	{
		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Enemy");
		
		auto enemyRenderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
		enemyRenderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureShared("ball"));
		enemyRenderer->SetPixelSize(32, 32);

		auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
		transform->SetLocalPosition(400.0f, 300.0f);

		auto body = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
		body->SetLinearDamping(2.0f);

		auto collider = gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

		auto ai = gameObject->AddComponent<XYZEngine::EnemyAIComponent>();
		ai->SetDetectionRadius(150.0f);
		ai->SetMovementSpeed(1.0f); // Медленнее игрока: 1.7f * 0.6 ≈ 1.0f
	}

	XYZEngine::GameObject* Enemy::GetGameObject()
	{
		return gameObject;
	}

	void Enemy::SetPlayer(XYZEngine::GameObject* player)
	{
		auto ai = gameObject->GetComponent<XYZEngine::EnemyAIComponent>();
		if (ai)
		{
			ai->SetPlayer(player);
		}
	}
}
