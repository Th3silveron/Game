#include "Enemy.h"
#include <ResourceSystem.h>
#include <SpriteColliderComponent.h>
#include <EnemyAIComponent.h>
#include <GameWorld.h>
#include <SpriteRendererComponent.h>
#include <DeathComponent.h>
#include <Logger.h>

using namespace XYZEngine;

namespace XYZRoguelike
{
	Enemy::Enemy()
	{
		LOG_INFO("Creating enemy...", "ENEMY");
		gameObject = GameWorld::Instance()->CreateGameObject("Enemy");
		ASSERT_LOG(gameObject != nullptr, "Failed to create enemy GameObject", "ENEMY");
		
		auto enemyRenderer = gameObject->AddComponent<SpriteRendererComponent>();
		enemyRenderer->SetTexture(*ResourceSystem::Instance()->GetTextureShared("ball"));
		enemyRenderer->SetPixelSize(128, 128); // Увеличиваем размер врага в 2 раза: 64 -> 128

		auto transform = gameObject->GetComponent<TransformComponent>();
		transform->SetLocalPosition(1600.0f, 1200.0f); // Позиция врага в большой карте

		auto body = gameObject->AddComponent<RigidbodyComponent>();
		body->SetLinearDamping(2.0f);
		body->SetKinematic(false); // Враг НЕ кинематический, но специальная логика предотвращает толкание

		auto collider = gameObject->AddComponent<SpriteColliderComponent>();

		auto ai = gameObject->AddComponent<EnemyAIComponent>();
		ai->SetDetectionRadius(600.0f); // Увеличиваем радиус обнаружения в 2 раза: 300->600
		ai->SetMovementSpeed(1.0f); // Медленнее игрока: 1.7f * 0.6 ≈ 1.0f

		// Добавляем систему здоровья (на 50% меньше чем у игрока)
		LOG_INFO("Setting up enemy health system (50 HP, 25 AP - 50% of player)", "ENEMY");
		healthComponent = gameObject->AddComponent<HealthComponent>();
		healthComponent->SetMaxHealth(50.0f);   // 50% от здоровья игрока (100/2 = 50)
		healthComponent->SetMaxArmor(25.0f);    // 50% от брони игрока (50/2 = 25)
		healthComponent->RestoreFullHealth();
		healthComponent->RestoreFullArmor();

		// Добавляем полоски здоровья
		auto healthBar = gameObject->AddComponent<HealthBarComponent>();
		healthBar->SetHealthComponent(healthComponent);
		healthBar->SetHealthBarSize(240.0f, 32.0f); // Увеличиваем в 2 раза: 120->240, 16->32
		healthBar->SetHealthBarOffset(0.0f, -100.0f); // Увеличиваем отступ: -50->-100

		// Добавляем боевую систему
		LOG_INFO("Setting up enemy combat system (15 damage, 140 range, 1.0s cooldown)", "ENEMY");
		combatComponent = gameObject->AddComponent<CombatComponent>();
		combatComponent->SetDamage(15.0f);      // Урон врага (меньше чем у игрока)
		combatComponent->SetAttackRange(140.0f); // Дальность атаки (увеличиваем в 2 раза: 70->140)
		combatComponent->SetAttackCooldown(1.0f); // Кулдаун атаки

		// Связываем AI с боевой системой
		ai->SetCombatComponent(combatComponent);

		// Добавляем компонент смерти
		LOG_INFO("Setting up enemy death system", "ENEMY");
		auto deathComponent = gameObject->AddComponent<DeathComponent>();
		deathComponent->SetDestroyOnDeath(true);
		deathComponent->OnDeath = []() {
			LOG_INFO("Enemy died!", "ENEMY");
		};
		
		LOG_INFO("Enemy creation completed successfully", "ENEMY");
	}

	GameObject* Enemy::GetGameObject()
	{
		return gameObject;
	}

	void Enemy::SetPlayer(GameObject* player)
	{
		auto ai = gameObject->GetComponent<EnemyAIComponent>();
		if (ai)
		{
			ai->SetPlayer(player);
		}
	}

	HealthComponent* Enemy::GetHealthComponent()
	{
		return healthComponent;
	}

	CombatComponent* Enemy::GetCombatComponent()
	{
		return combatComponent;
	}

	bool Enemy::IsAlive() const
	{
		return healthComponent && healthComponent->IsAlive();
	}
}
