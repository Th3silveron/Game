#pragma once

#include <CameraComponent.h>
#include <GameWorld.h>
#include <SpriteRendererComponent.h>
#include <RenderSystem.h>
#include <InputComponent.h>
#include <GameObject.h>
#include <HealthComponent.h>
#include <HealthBarComponent.h>
#include <CombatComponent.h>

namespace XYZRoguelike
{
	class Player
	{
	public:
		Player();
		XYZEngine::GameObject* GetGameObject();
		XYZEngine::HealthComponent* GetHealthComponent();
		XYZEngine::CombatComponent* GetCombatComponent();
		bool IsAlive() const;
	private:
		XYZEngine::GameObject* gameObject;
		XYZEngine::HealthComponent* healthComponent;
		XYZEngine::CombatComponent* combatComponent;
	};
}