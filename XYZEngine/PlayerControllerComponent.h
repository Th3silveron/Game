#pragma once

#include "Component.h"

namespace XYZEngine
{
	class InputComponent;
	class RigidbodyComponent;
	class CombatComponent;
	class GameObject;

	class PlayerControllerComponent : public Component
	{
	public:
		PlayerControllerComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetMovementSpeed(float speed);
		float GetMovementSpeed() const;

		void SetCombatComponent(CombatComponent* combatComponent);
		void SetEnemies(const std::vector<GameObject*>& enemies);

	private:
		void TryAttack();

		InputComponent* inputComponent;
		RigidbodyComponent* rigidbodyComponent;
		CombatComponent* combatComponent;
		std::vector<GameObject*> enemies;
		float movementSpeed = 200.0f;
	};
}
