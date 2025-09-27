#include "pch.h"
#include "EnemyAIComponent.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"

namespace XYZEngine
{
	EnemyAIComponent::EnemyAIComponent(GameObject* gameObject)
		: Component(gameObject), player(nullptr)
	{
		transform = gameObject->GetComponent<TransformComponent>();
		rigidbody = gameObject->GetComponent<RigidbodyComponent>();
	}

	void EnemyAIComponent::Update(float deltaTime)
	{
		if (!transform || !rigidbody || !player)
			return;

		TransformComponent* playerTransform = player->GetComponent<TransformComponent>();
		if (!playerTransform)
			return;

		float distance = CalculateDistanceToPlayer();
		isPlayerDetected = distance <= detectionRadius;

		if (isPlayerDetected)
		{
			Vector2Df direction = CalculateDirectionToPlayer();
			Vector2Df velocity = direction * movementSpeed;
			rigidbody->SetLinearVelocity(velocity);
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
}
