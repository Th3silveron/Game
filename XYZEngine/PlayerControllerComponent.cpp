#include "pch.h"
#include "PlayerControllerComponent.h"
#include "InputComponent.h"
#include "RigidbodyComponent.h"
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
}
