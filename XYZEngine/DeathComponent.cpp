#include "pch.h"
#include "DeathComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "PlayerControllerComponent.h"
#include "EnemyAIComponent.h"
#include "Logger.h"
#include <iostream>

namespace XYZEngine
{
	DeathComponent::DeathComponent(GameObject* gameObject)
		: Component(gameObject)
		, healthComponent(nullptr)
		, destroyOnDeath(true)
		, hideOnDeath(false)
		, disableComponentsOnDeath(false)
		, wasAlive(true)
	{
		LOG_DEBUG("DeathComponent created for GameObject: '" + gameObject->GetName() + "'", "DEATH");
		
		healthComponent = gameObject->GetComponent<HealthComponent>();
		if (healthComponent)
		{
			wasAlive = healthComponent->IsAlive();
		}
		else
		{
			LOG_WARNING("DeathComponent created without HealthComponent for: '" + gameObject->GetName() + "'", "DEATH");
		}
	}

	DeathComponent::~DeathComponent()
	{
	}

	void DeathComponent::Update(float deltaTime)
	{
		if (!healthComponent)
		{
			healthComponent = gameObject->GetComponent<HealthComponent>();
			if (healthComponent)
			{
				wasAlive = healthComponent->IsAlive();
			}
			return;
		}

		bool isCurrentlyAlive = healthComponent->IsAlive();
		
		// Проверяем, изменилось ли состояние (жив -> мертв)
		if (wasAlive && !isCurrentlyAlive)
		{
			HandleDeath();
		}
		
		wasAlive = isCurrentlyAlive;
	}

	void DeathComponent::Render()
	{
		// Компонент смерти не рендерится напрямую
	}

	void DeathComponent::SetDestroyOnDeath(bool destroy)
	{
		destroyOnDeath = destroy;
	}

	void DeathComponent::SetHideOnDeath(bool hide)
	{
		hideOnDeath = hide;
	}

	void DeathComponent::SetDisableComponentsOnDeath(bool disable)
	{
		disableComponentsOnDeath = disable;
	}

	void DeathComponent::HandleDeath()
	{
		LOG_WARNING("GameObject died: '" + gameObject->GetName() + "'", "DEATH");

		// Вызываем событие смерти
		if (OnDeath)
		{
			OnDeath();
		}

		// Выполняем действия при смерти
		if (destroyOnDeath)
		{
			LOG_INFO("Destroying GameObject: '" + gameObject->GetName() + "'", "DEATH");
			DestroyGameObject();
		}
		else if (hideOnDeath)
		{
			LOG_INFO("Hiding GameObject: '" + gameObject->GetName() + "'", "DEATH");
			HideGameObject();
		}
		
		if (disableComponentsOnDeath)
		{
			DisableComponents();
		}
	}

	void DeathComponent::DestroyGameObject()
	{
		// Скрываем объект, делая его прозрачным
		auto renderer = gameObject->GetComponent<SpriteRendererComponent>();
		if (renderer)
		{
			// Делаем объект полностью прозрачным
			renderer->SetColor(sf::Color(255, 255, 255, 0));
		}
		
		std::cout << "GameObject " << gameObject->GetName() << " destroyed!" << std::endl;
	}

	void DeathComponent::HideGameObject()
	{
		// Скрываем объект, делая его прозрачным
		auto renderer = gameObject->GetComponent<SpriteRendererComponent>();
		if (renderer)
		{
			// Делаем объект полностью прозрачным
			renderer->SetColor(sf::Color(255, 255, 255, 0));
		}
	}

	void DeathComponent::DisableComponents()
	{
		// В данной версии просто скрываем объект
		// В будущем можно добавить методы отключения компонентов
		HideGameObject();
	}
}
