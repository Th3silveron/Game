#include "pch.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

namespace XYZEngine
{
	CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
	{
		view = new sf::View(sf::FloatRect(0, 0, 800, 600));
		transform = gameObject->GetComponent<TransformComponent>();
		smoothFollow = true;
		smoothFactor = 5.0f;
		targetPosition = { 0, 0 };
		
		// Инициализируем камеру в позиции игрока
		if (transform)
		{
			auto position = transform->GetWorldPosition();
			targetPosition = position;
			view->setCenter(Convert<sf::Vector2f, Vector2Df>(position));
		}
	}
	CameraComponent::~CameraComponent()
	{
		delete view;
	}

	void CameraComponent::Update(float deltaTime)
	{
		auto position = transform->GetWorldPosition();
		auto rotation = transform->GetWorldRotation();

		if (smoothFollow)
		{
			// Сглаживаем движение камеры
			Vector2Df currentCenter = { view->getCenter().x, view->getCenter().y };
			Vector2Df direction = position - currentCenter;
			targetPosition = currentCenter + direction * smoothFactor * deltaTime;
			
			view->setCenter(Convert<sf::Vector2f, Vector2Df>(targetPosition));
		}
		else
		{
			view->setCenter(Convert<sf::Vector2f, Vector2Df>(position));
		}
		
		view->setRotation(rotation);
		window->setView(*view);
	}
	void CameraComponent::Render()
	{
		if (window == nullptr)
		{
			std::cout << "NULL window render." << std::endl;
		}
	}

	void CameraComponent::SetBaseResolution(int width, int height)
	{
		view->reset(sf::FloatRect(0, 0, static_cast<float>(width), static_cast<float>(height)));
	}
	
	void CameraComponent::SetSmoothFollow(bool enabled, float smoothFactor)
	{
		this->smoothFollow = enabled;
		this->smoothFactor = smoothFactor;
	}
	void CameraComponent::SetWindow(sf::RenderWindow* newWindow)
	{
		window = newWindow;
	}
	void CameraComponent::ZoomBy(float newZoom)
	{
		if (newZoom <= 0)
		{
			std::cout << "Not allowed zoom lesser or equal than zero." << std::endl;
			return;
		}
		view->zoom(newZoom);
	}
}