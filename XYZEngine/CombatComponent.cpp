#include "pch.h"
#include "CombatComponent.h"
#include "Logger.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "Vector.h"
#include <cmath>
#include <SFML/System.hpp>

namespace XYZEngine
{
	CombatComponent::CombatComponent(GameObject* gameObject)
		: Component(gameObject)
		, damage(20.0f)
		, attackRange(50.0f)
		, attackCooldown(1.0f)
		, attackSpeed(1.0f)
		, lastAttackTime(0.0f)
	{
		// Инициализируем время последней атаки
		lastAttackTime = -attackCooldown; // Позволяем атаковать сразу
		LOG_DEBUG("CombatComponent created for GameObject: '" + gameObject->GetName() + "'", "COMBAT");
	}

	CombatComponent::~CombatComponent()
	{
	}

	void CombatComponent::Update(float deltaTime)
	{
		// Обновляем время последней атаки
		lastAttackTime += deltaTime;
	}

	void CombatComponent::Render()
	{
		// Компонент боя не рендерится напрямую
	}

	void CombatComponent::SetDamage(float damage)
	{
		this->damage = damage;
	}

	void CombatComponent::SetAttackRange(float range)
	{
		this->attackRange = range;
	}

	void CombatComponent::SetAttackCooldown(float cooldown)
	{
		this->attackCooldown = cooldown;
	}

	void CombatComponent::SetAttackSpeed(float speed)
	{
		this->attackSpeed = speed;
	}

	float CombatComponent::GetDamage() const
	{
		return damage;
	}

	float CombatComponent::GetAttackRange() const
	{
		return attackRange;
	}

	float CombatComponent::GetAttackCooldown() const
	{
		return attackCooldown;
	}

	float CombatComponent::GetAttackSpeed() const
	{
		return attackSpeed;
	}

	void CombatComponent::Attack(GameObject* target)
	{
		ASSERT_LOG(target != nullptr, "Attack target cannot be null", "COMBAT");
		
		if (!CanAttack() || !target)
		{
			LOG_DEBUG("Attack failed - cannot attack or target is null", "COMBAT");
			return;
		}

		auto targetHealth = target->GetComponent<HealthComponent>();
		if (!targetHealth)
		{
			LOG_WARNING("Attack failed - target has no HealthComponent", "COMBAT");
			return;
		}

		LOG_INFO("Attacking target: '" + target->GetName() + "' for " + std::to_string(damage) + " damage", "COMBAT");
		PerformAttack(targetHealth, target);
	}

	void CombatComponent::Attack(HealthComponent* targetHealth)
	{
		if (!CanAttack() || !targetHealth)
			return;

		PerformAttack(targetHealth, nullptr);
	}

	bool CombatComponent::CanAttack() const
	{
		// Проверяем кулдаун атаки
		return lastAttackTime >= attackCooldown;
	}

	bool CombatComponent::IsInRange(GameObject* target) const
	{
		if (!target)
			return false;

		auto myTransform = gameObject->GetComponent<TransformComponent>();
		auto targetTransform = target->GetComponent<TransformComponent>();

		if (!myTransform || !targetTransform)
			return false;

		Vector2D myPos = myTransform->GetWorldPosition();
		Vector2D targetPos = targetTransform->GetWorldPosition();

		float distance = static_cast<float>(std::sqrt(
			std::pow(static_cast<double>(myPos.x - targetPos.x), 2) + 
			std::pow(static_cast<double>(myPos.y - targetPos.y), 2)
		));

		return distance <= attackRange;
	}

	void CombatComponent::PerformAttack(HealthComponent* targetHealth, GameObject* target)
	{
		if (!targetHealth || targetHealth->IsDead())
			return;

		// Наносим урон
		targetHealth->TakeDamage(damage);

		// Сбрасываем время последней атаки
		lastAttackTime = 0.0f;

		// Вызываем события
		if (OnAttackPerformed && target)
		{
			OnAttackPerformed(target);
		}

		// Проверяем, был ли убит цель
		if (targetHealth->IsDead() && OnTargetKilled && target)
		{
			OnTargetKilled(target);
		}
	}
}
