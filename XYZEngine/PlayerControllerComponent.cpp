#include "pch.h"
#include "PlayerControllerComponent.h"
#include "InputComponent.h"
#include "RigidbodyComponent.h"
#include "CombatComponent.h"
#include "HealthComponent.h"
#include "Vector.h"

namespace XYZEngine
{
	PlayerControllerComponent::PlayerControllerComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		inputComponent = gameObject->GetComponent<InputComponent>();
		rigidbodyComponent = gameObject->GetComponent<RigidbodyComponent>();
	}

	void PlayerControllerComponent::Update(float deltaTime)
	{
		if (!inputComponent || !rigidbodyComponent)
			return;

		float horizontalAxis = inputComponent->GetHorizontalAxis();
		float verticalAxis = inputComponent->GetVerticalAxis();

		Vector2Df movement(horizontalAxis * movementSpeed, verticalAxis * movementSpeed);
		rigidbodyComponent->SetLinearVelocity(movement);

		// Проверяем атаку
		TryAttack();
	}

	void PlayerControllerComponent::Render()
	{
		// PlayerController component doesn't need render logic
	}

	void PlayerControllerComponent::SetMovementSpeed(float speed)
	{
		movementSpeed = speed;
	}

	float PlayerControllerComponent::GetMovementSpeed() const
	{
		return movementSpeed;
	}

	void PlayerControllerComponent::SetCombatComponent(CombatComponent* combatComponent)
	{
		this->combatComponent = combatComponent;
	}

	void PlayerControllerComponent::SetEnemies(const std::vector<GameObject*>& enemies)
	{
		this->enemies = enemies;
	}

	void PlayerControllerComponent::TryAttack()
	{
		if (!combatComponent || !inputComponent)
			return;

		// Проверяем нажатие клавиши атаки (например, пробел)
		if (inputComponent->IsKeyPressed(sf::Keyboard::Space))
		{
			// Ищем ближайшего врага в радиусе атаки
			for (auto enemy : enemies)
			{
				if (enemy && combatComponent->IsInRange(enemy))
				{
					auto enemyHealth = enemy->GetComponent<HealthComponent>();
					if (enemyHealth && enemyHealth->IsAlive())
					{
						combatComponent->Attack(enemy);
						break; // Атакуем только одного врага за раз
					}
				}
			}
		}
	}
}
