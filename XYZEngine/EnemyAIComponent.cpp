#include "pch.h"
#include "EnemyAIComponent.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "CombatComponent.h"
#include "HealthComponent.h"

namespace XYZEngine
{
	EnemyAIComponent::EnemyAIComponent(GameObject* gameObject)
		: Component(gameObject), player(nullptr), combatComponent(nullptr)
	{
		transform = gameObject->GetComponent<TransformComponent>();
		rigidbody = gameObject->GetComponent<RigidbodyComponent>();
		lastAttackTime = -1.0f; // Позволяем атаковать сразу
	}

	void EnemyAIComponent::Update(float deltaTime)
	{
		if (!transform || !rigidbody || !player)
			return;

		TransformComponent* playerTransform = player->GetComponent<TransformComponent>();
		if (!playerTransform)
			return;

		// Обновляем время последней атаки
		lastAttackTime += deltaTime;

		float distance = CalculateDistanceToPlayer();
		isPlayerDetected = distance <= detectionRadius;

		if (isPlayerDetected)
		{
			float minStopDistance = 5.0f; // Минимальное расстояние до игрока
			float attackRange = combatComponent ? combatComponent->GetAttackRange() : 35.0f;
			
			if (distance <= minStopDistance)
			{
				// Если слишком близко к игроку, останавливаемся
				rigidbody->SetLinearVelocity(Vector2Df(0.0f, 0.0f));
				TryAttack();
			}
			else if (distance <= attackRange)
			{
				// Если в радиусе атаки, но не слишком близко, останавливаемся и атакуем
				rigidbody->SetLinearVelocity(Vector2Df(0.0f, 0.0f));
				TryAttack();
			}
			else
			{
				// Если далеко от игрока, приближаемся
				Vector2Df direction = CalculateDirectionToPlayer();
				Vector2Df velocity = direction * movementSpeed;
				rigidbody->SetLinearVelocity(velocity);
			}
		}
		else
		{
			rigidbody->SetLinearVelocity(Vector2Df(0.0f, 0.0f));
		}
	}

	void EnemyAIComponent::Render()
	{
		// AI component doesn't need render logic
	}

	void EnemyAIComponent::SetPlayer(GameObject* player)
	{
		this->player = player;
	}

	void EnemyAIComponent::SetDetectionRadius(float radius)
	{
		detectionRadius = radius;
	}

	void EnemyAIComponent::SetMovementSpeed(float speed)
	{
		movementSpeed = speed;
	}

	void EnemyAIComponent::SetCombatComponent(CombatComponent* combatComponent)
	{
		this->combatComponent = combatComponent;
	}

	float EnemyAIComponent::GetDetectionRadius() const
	{
		return detectionRadius;
	}

	float EnemyAIComponent::GetMovementSpeed() const
	{
		return movementSpeed;
	}

	bool EnemyAIComponent::IsPlayerDetected() const
	{
		return isPlayerDetected;
	}

	Vector2Df EnemyAIComponent::CalculateDirectionToPlayer() const
	{
		if (!transform || !player)
			return Vector2Df(0.0f, 0.0f);

		TransformComponent* playerTransform = player->GetComponent<TransformComponent>();
		if (!playerTransform)
			return Vector2Df(0.0f, 0.0f);

		Vector2Df enemyPos = transform->GetLocalPosition();
		Vector2Df playerPos = playerTransform->GetLocalPosition();

		Vector2Df direction = playerPos - enemyPos;
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);

		if (length > 0.0f)
		{
			direction.x /= length;
			direction.y /= length;
		}

		return direction;
	}

	float EnemyAIComponent::CalculateDistanceToPlayer() const
	{
		if (!transform || !player)
			return 0.0f;

		TransformComponent* playerTransform = player->GetComponent<TransformComponent>();
		if (!playerTransform)
			return 0.0f;

		Vector2Df enemyPos = transform->GetLocalPosition();
		Vector2Df playerPos = playerTransform->GetLocalPosition();

		Vector2Df distance = playerPos - enemyPos;
		return sqrt(distance.x * distance.x + distance.y * distance.y);
	}

	void EnemyAIComponent::TryAttack()
	{
		if (!combatComponent || !player)
			return;

		// Проверяем, жив ли сам враг
		auto myHealth = gameObject->GetComponent<HealthComponent>();
		if (!myHealth || !myHealth->IsAlive())
			return;

		// Проверяем кулдаун атаки
		if (lastAttackTime < combatComponent->GetAttackCooldown())
			return;

		// Проверяем, жив ли игрок
		auto playerHealth = player->GetComponent<HealthComponent>();
		if (playerHealth && playerHealth->IsAlive())
		{
			combatComponent->Attack(player);
			lastAttackTime = 0.0f; // Сбрасываем время последней атаки
		}
	}
}
