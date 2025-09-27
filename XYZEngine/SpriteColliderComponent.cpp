#include "pch.h"
#include "SpriteColliderComponent.h"

namespace XYZEngine
{
	SpriteColliderComponent::SpriteColliderComponent(GameObject* gameObject) : ColliderComponent(gameObject)
	{
		auto spriteRenderer = gameObject->GetComponent<SpriteRendererComponent>();
		if (spriteRenderer == nullptr)
		{
			std::cout << "ERROR: SpriteRenderer required to SpriteCollider for " << gameObject->GetName() << std::endl;
			gameObject->RemoveComponent(this);
			return;
		}

		sprite = gameObject->GetComponent<SpriteRendererComponent>()->GetSprite();
		if (sprite)
		{
			PhysicsSystem::Instance()->Subscribe(this);
			// std::cout << "SpriteColliderComponent created for " << gameObject->GetName() << std::endl;
		}
		else
		{
			std::cout << "ERROR: Sprite is null in SpriteRendererComponent for " << gameObject->GetName() << std::endl;
		}
	}
	SpriteColliderComponent::~SpriteColliderComponent()
	{
		if (&bounds != nullptr)
		{
			std::destroy_at(&bounds);
		}
		PhysicsSystem::Instance()->Unsubscribe(this);
		// std::cout << "SpriteColliderComponent destroyed for " << gameObject->GetName() << std::endl;
	}

	void SpriteColliderComponent::Update(float deltaTime)
	{
		if (sprite)
		{
			bounds = sprite->getGlobalBounds();
			
			// Отладочный вывод для проверки bounds (отключен)
			// static int debugCounter = 0;
			// if (debugCounter++ % 60 == 0) // Каждую секунду (при 60 FPS)
			// {
			//	std::cout << "Collider " << gameObject->GetName() << " bounds: " 
			//			  << bounds.left << ", " << bounds.top 
			//			  << ", " << bounds.width << ", " << bounds.height << std::endl;
			//			  
			//	// Проверяем, что bounds не пустые
			//	if (bounds.width <= 0 || bounds.height <= 0)
			//	{
			//		std::cout << "WARNING: " << gameObject->GetName() << " has invalid bounds!" << std::endl;
			//	}
			// }
		}
		else
		{
			std::cout << "ERROR: Sprite is null in SpriteColliderComponent for " << gameObject->GetName() << "!" << std::endl;
		}
	}
	void SpriteColliderComponent::Render()
	{
		// Отладочная отрисовка коллайдеров отключена
		// sf::RectangleShape rectangle(sf::Vector2f(bounds.width, bounds.height));
		// rectangle.setPosition(bounds.left, bounds.top);
		// rectangle.setFillColor(sf::Color::Transparent);
		// rectangle.setOutlineColor(sf::Color::Yellow);
		// rectangle.setOutlineThickness(2);
		// RenderSystem::Instance()->Render(rectangle);
	}
}