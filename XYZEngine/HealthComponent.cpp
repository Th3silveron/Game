#include "pch.h"
#include "HealthComponent.h"
#include "Logger.h"
#include "GameObject.h"

namespace XYZEngine
{
	HealthComponent::HealthComponent(GameObject* gameObject)
		: Component(gameObject)
		, currentHealth(100.0f)
		, maxHealth(100.0f)
		, currentArmor(50.0f)
		, maxArmor(50.0f)
		, isAlive(true)
	{
		LOG_DEBUG("HealthComponent created for GameObject: '" + gameObject->GetName() + "'", "HEALTH");
	}

	HealthComponent::~HealthComponent()
	{
	}

	void HealthComponent::Update(float deltaTime)
	{
		// Компонент здоровья не требует обновления каждый кадр
		// Логика обновления может быть добавлена здесь при необходимости
	}

	void HealthComponent::Render()
	{
		// Компонент здоровья не рендерится напрямую
		// Рендеринг полосок здоровья должен быть в отдельном компоненте
	}

	void HealthComponent::SetMaxHealth(float maxHealth)
	{
		this->maxHealth = maxHealth;
		if (currentHealth > maxHealth)
		{
			currentHealth = maxHealth;
		}
	}

	void HealthComponent::SetMaxArmor(float maxArmor)
	{
		this->maxArmor = maxArmor;
		if (currentArmor > maxArmor)
		{
			currentArmor = maxArmor;
		}
	}

	float HealthComponent::GetCurrentHealth() const
	{
		return currentHealth;
	}

	float HealthComponent::GetMaxHealth() const
	{
		return maxHealth;
	}

	float HealthComponent::GetCurrentArmor() const
	{
		return currentArmor;
	}

	float HealthComponent::GetMaxArmor() const
	{
		return maxArmor;
	}

	float HealthComponent::GetHealthPercentage() const
	{
		if (maxHealth <= 0.0f) return 0.0f;
		return currentHealth / maxHealth;
	}

	float HealthComponent::GetArmorPercentage() const
	{
		if (maxArmor <= 0.0f) return 0.0f;
		return currentArmor / maxArmor;
	}

	void HealthComponent::RestoreHealth(float amount)
	{
		currentHealth += amount;
		if (currentHealth > maxHealth)
		{
			currentHealth = maxHealth;
		}
		if (OnHealthChanged)
		{
			OnHealthChanged(currentHealth);
		}
	}

	void HealthComponent::RestoreArmor(float amount)
	{
		currentArmor += amount;
		if (currentArmor > maxArmor)
		{
			currentArmor = maxArmor;
		}
		if (OnArmorChanged)
		{
			OnArmorChanged(currentArmor);
		}
	}

	void HealthComponent::RestoreFullHealth()
	{
		currentHealth = maxHealth;
		if (OnHealthChanged)
		{
			OnHealthChanged(currentHealth);
		}
	}

	void HealthComponent::RestoreFullArmor()
	{
		currentArmor = maxArmor;
		if (OnArmorChanged)
		{
			OnArmorChanged(currentArmor);
		}
	}

	void HealthComponent::TakeDamage(float damage)
	{
		ASSERT_LOG(damage >= 0.0f, "Damage cannot be negative", "HEALTH");
		
		if (!isAlive || damage <= 0.0f) 
		{
			LOG_DEBUG("TakeDamage ignored - object not alive or no damage", "HEALTH");
			return;
		}

		LOG_INFO("Taking damage: " + std::to_string(damage) + " (Object: '" + gameObject->GetName() + "')", "HEALTH");
		
		float remainingDamage = damage;
		float initialHealth = currentHealth;
		float initialArmor = currentArmor;

		// Сначала урон идет на броню
		if (currentArmor > 0.0f)
		{
			float armorDamage = std::min(currentArmor, remainingDamage);
			currentArmor -= armorDamage;
			remainingDamage -= armorDamage;

			LOG_DEBUG("Armor damaged: " + std::to_string(armorDamage) + " (remaining: " + std::to_string(currentArmor) + ")", "HEALTH");

			if (OnArmorChanged)
			{
				OnArmorChanged(currentArmor);
			}
		}

		// Оставшийся урон идет на здоровье
		if (remainingDamage > 0.0f && currentHealth > 0.0f)
		{
			currentHealth -= remainingDamage;
			if (currentHealth < 0.0f)
			{
				currentHealth = 0.0f;
			}

			LOG_DEBUG("Health damaged: " + std::to_string(remainingDamage) + " (remaining: " + std::to_string(currentHealth) + ")", "HEALTH");

			if (OnHealthChanged)
			{
				OnHealthChanged(currentHealth);
			}
		}

		// Проверяем смерть
		if (currentHealth <= 0.0f && isAlive)
		{
			isAlive = false;
			LOG_WARNING("Object died: '" + gameObject->GetName() + "' (Health: " + std::to_string(currentHealth) + ")", "HEALTH");
			if (OnDeath)
			{
				OnDeath();
			}
		}
	}

	bool HealthComponent::IsAlive() const
	{
		return isAlive;
	}

	bool HealthComponent::IsDead() const
	{
		return !isAlive;
	}
}
