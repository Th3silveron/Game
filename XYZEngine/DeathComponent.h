#pragma once

#include "pch.h"
#include "Component.h"
#include "HealthComponent.h"

namespace XYZEngine
{
	class DeathComponent : public Component
	{
	public:
		DeathComponent(GameObject* gameObject);
		virtual ~DeathComponent();

		virtual void Update(float deltaTime) override;
		virtual void Render() override;

		// Настройка поведения при смерти
		void SetDestroyOnDeath(bool destroy);
		void SetHideOnDeath(bool hide);
		void SetDisableComponentsOnDeath(bool disable);

		// События
		std::function<void()> OnDeath;

	private:
		void HandleDeath();
		void DestroyGameObject();
		void HideGameObject();
		void DisableComponents();

		HealthComponent* healthComponent;
		bool destroyOnDeath;
		bool hideOnDeath;
		bool disableComponentsOnDeath;
		bool wasAlive;
	};
}


