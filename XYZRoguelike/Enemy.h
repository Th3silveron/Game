#pragma once

#include <GameObject.h>
#include <HealthComponent.h>
#include <HealthBarComponent.h>
#include <CombatComponent.h>

namespace XYZRoguelike
{
	class Enemy
	{
	public:
		Enemy();
		XYZEngine::GameObject* GetGameObject();
		void SetPlayer(XYZEngine::GameObject* player);
		XYZEngine::HealthComponent* GetHealthComponent();
		XYZEngine::CombatComponent* GetCombatComponent();
		bool IsAlive() const;

	private:
		XYZEngine::GameObject* gameObject;
		XYZEngine::HealthComponent* healthComponent;
		XYZEngine::CombatComponent* combatComponent;
	};
}
