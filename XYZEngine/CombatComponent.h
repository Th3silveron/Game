#pragma once

#include "pch.h"
#include "Component.h"
#include <vector>
#include <functional>

namespace XYZEngine
{
	class HealthComponent;

	class CombatComponent : public Component
	{
	public:
		CombatComponent(GameObject* gameObject);
		virtual ~CombatComponent();

		virtual void Update(float deltaTime) override;
		virtual void Render() override;

		// Настройка боевых параметров
		void SetDamage(float damage);
		void SetAttackRange(float range);
		void SetAttackCooldown(float cooldown);
		void SetAttackSpeed(float speed);

		// Получение параметров
		float GetDamage() const;
		float GetAttackRange() const;
		float GetAttackCooldown() const;
		float GetAttackSpeed() const;

		// Атака
		void Attack(GameObject* target);
		void Attack(HealthComponent* targetHealth);

		// Проверки
		bool CanAttack() const;
		bool IsInRange(GameObject* target) const;

		// События
		std::function<void(GameObject*)> OnAttackPerformed;
		std::function<void(GameObject*)> OnTargetKilled;

	private:
		float damage;
		float attackRange;
		float attackCooldown;
		float attackSpeed;
		float lastAttackTime;

		void PerformAttack(HealthComponent* targetHealth, GameObject* target);
	};
}
