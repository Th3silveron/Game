#pragma once

#include "pch.h"
#include "Component.h"
#include <functional>

namespace XYZEngine
{
	// Forward declaration
	class GameObject;
	class HealthComponent : public Component
	{
	public:
		HealthComponent(GameObject* gameObject);
		virtual ~HealthComponent();

		virtual void Update(float deltaTime) override;
		virtual void Render() override;

		// Установка максимальных значений
		void SetMaxHealth(float maxHealth);
		void SetMaxArmor(float maxArmor);

		// Получение текущих значений
		float GetCurrentHealth() const;
		float GetMaxHealth() const;
		float GetCurrentArmor() const;
		float GetMaxArmor() const;

		// Получение процентов (0.0 - 1.0)
		float GetHealthPercentage() const;
		float GetArmorPercentage() const;

		// Восстановление
		void RestoreHealth(float amount);
		void RestoreArmor(float amount);
		void RestoreFullHealth();
		void RestoreFullArmor();

		// Получение урона
		void TakeDamage(float damage);

		// Проверки состояния
		bool IsAlive() const;
		bool IsDead() const;

		// События
		std::function<void(float)> OnHealthChanged;
		std::function<void(float)> OnArmorChanged;
		std::function<void()> OnDeath;

	private:
		float currentHealth;
		float maxHealth;
		float currentArmor;
		float maxArmor;
		bool isAlive;
	};
}
