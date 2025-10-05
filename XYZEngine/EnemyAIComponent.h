#pragma once

#include "Component.h"
#include "Vector.h"

namespace XYZEngine
{
	class TransformComponent;
	class RigidbodyComponent;
	class CombatComponent;
	class HealthComponent;

	class EnemyAIComponent : public Component
	{
	public:
		EnemyAIComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetPlayer(GameObject* player);
		void SetDetectionRadius(float radius);
		void SetMovementSpeed(float speed);
		void SetCombatComponent(CombatComponent* combatComponent);

		float GetDetectionRadius() const;
		float GetMovementSpeed() const;
		bool IsPlayerDetected() const;

	private:
		void TryAttack();

		TransformComponent* transform;
		RigidbodyComponent* rigidbody;
		CombatComponent* combatComponent;

		GameObject* player;
		float detectionRadius = 200.0f;
		float movementSpeed = 100.0f;
		bool isPlayerDetected = false;
		float lastAttackTime = 0.0f;

		Vector2Df CalculateDirectionToPlayer() const;
		float CalculateDistanceToPlayer() const;
	};
}
