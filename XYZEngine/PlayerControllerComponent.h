#pragma once

#include "Component.h"

namespace XYZEngine
{
	class InputComponent;
	class RigidbodyComponent;

	class PlayerControllerComponent : public Component
	{
	public:
		PlayerControllerComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetMovementSpeed(float speed);
		float GetMovementSpeed() const;

	private:
		InputComponent* inputComponent;
		RigidbodyComponent* rigidbodyComponent;
		float movementSpeed = 200.0f;
	};
}
