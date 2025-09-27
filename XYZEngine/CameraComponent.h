#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "TransformComponent.h"

namespace XYZEngine
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(GameObject* gameObject);
		~CameraComponent();

		void Update(float deltaTime) override;
		void Render() override;

		void SetWindow(sf::RenderWindow* newWindow);
		void ZoomBy(float newZoom);

		void SetBaseResolution(int width, int height);
		void SetSmoothFollow(bool enabled, float smoothFactor = 5.0f);
	private:
		TransformComponent* transform;
		sf::RenderWindow* window;
		sf::View* view;
		bool smoothFollow;
		float smoothFactor;
		Vector2Df targetPosition;
	};
}