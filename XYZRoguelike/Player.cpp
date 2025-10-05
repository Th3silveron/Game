#include "Player.h"
#include <ResourceSystem.h>
#include <SpriteColliderComponent.h>
#include <PlayerControllerComponent.h>
#include <DeathComponent.h>
#include <Logger.h>

using namespace XYZEngine;

namespace XYZRoguelike
{
	Player::Player()
	{
		LOG_INFO("Creating player...", "PLAYER");
		gameObject = GameWorld::Instance()->CreateGameObject("Player");
		ASSERT_LOG(gameObject != nullptr, "Failed to create player GameObject", "PLAYER");
		auto playerRenderer = gameObject->AddComponent<SpriteRendererComponent>();

		playerRenderer->SetTexture(*ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(128, 128); // Увеличиваем размер игрока в 2 раза: 64 -> 128
		playerRenderer->SetColor(sf::Color::Blue); // Синий цвет для игрока

		auto playerInput = gameObject->AddComponent<InputComponent>();

		auto transform = gameObject->GetComponent<TransformComponent>();
		transform->SetLocalPosition(2560.0f, 2560.0f); // Центр уровня (40*128/2 = 2560)

		auto playerCamera = gameObject->AddComponent<CameraComponent>();
		playerCamera->SetWindow(&RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1920, 1080); // Увеличенное разрешение для лучшего игрового опыта

		auto body = gameObject->AddComponent<RigidbodyComponent>();
		body->SetLinearDamping(5.0f); // Добавляем демпфирование для плавного движения
		body->SetKinematic(false); // Игрок НЕ кинематический, может двигаться и толкаться

		auto collider = gameObject->AddComponent<SpriteColliderComponent>();

		auto controller = gameObject->AddComponent<PlayerControllerComponent>();
		controller->SetMovementSpeed(1.2f); // Уменьшаем скорость для комфортной игры на большой карте: 1.7f -> 1.2f

		// Добавляем систему здоровья
		LOG_INFO("Setting up player health system (100 HP, 50 AP)", "PLAYER");
		healthComponent = gameObject->AddComponent<HealthComponent>();
		healthComponent->SetMaxHealth(100.0f);  // Максимальное здоровье игрока
		healthComponent->SetMaxArmor(50.0f);    // Максимальная броня игрока
		healthComponent->RestoreFullHealth();
		healthComponent->RestoreFullArmor();

		// Добавляем полоски здоровья
		auto healthBar = gameObject->AddComponent<HealthBarComponent>();
		healthBar->SetHealthComponent(healthComponent);
		healthBar->SetHealthBarSize(240.0f, 32.0f); // Увеличиваем в 2 раза: 120->240, 16->32
		healthBar->SetHealthBarOffset(0.0f, -100.0f); // Увеличиваем отступ: -50->-100

		// Добавляем боевую систему
		LOG_INFO("Setting up player combat system (25 damage, 160 range, 0.5s cooldown)", "PLAYER");
		combatComponent = gameObject->AddComponent<CombatComponent>();
		combatComponent->SetDamage(25.0f);      // Урон игрока
		combatComponent->SetAttackRange(160.0f); // Дальность атаки (увеличиваем в 2 раза: 80->160)
		combatComponent->SetAttackCooldown(0.5f); // Кулдаун атаки

		// Связываем контроллер с боевой системой
		controller->SetCombatComponent(combatComponent);

		// Добавляем компонент смерти
		LOG_INFO("Setting up player death system", "PLAYER");
		auto deathComponent = gameObject->AddComponent<DeathComponent>();
		deathComponent->SetDestroyOnDeath(true);
		deathComponent->OnDeath = []() {
			LOG_FATAL("PLAYER DIED! Game Over!", "PLAYER");
			// Завершаем игру
			XYZEngine::RenderSystem::Instance()->ExitGame();
		};
		
		LOG_INFO("Player creation completed successfully", "PLAYER");
	}

	GameObject* Player::GetGameObject()
	{
		return gameObject;
	}

	HealthComponent* Player::GetHealthComponent()
	{
		return healthComponent;
	}

	CombatComponent* Player::GetCombatComponent()
	{
		return combatComponent;
	}

	bool Player::IsAlive() const
	{
		return healthComponent && healthComponent->IsAlive();
	}
}