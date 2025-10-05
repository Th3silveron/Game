#include "pch.h"
#include "HealthBarComponent.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "RenderSystem.h"
#include "CameraComponent.h"
#include <SFML/Graphics.hpp>

namespace XYZEngine
{
	HealthBarComponent::HealthBarComponent(GameObject* gameObject)
		: Component(gameObject)
		, healthComponent(nullptr)
		, barWidth(60.0f)
		, barHeight(8.0f)
		, offsetX(0.0f)
		, offsetY(-25.0f)
		, healthColor(0, 255, 0, 200)    // Зеленый для здоровья
		, armorColor(0, 100, 255, 200)   // Синий для брони
		, backgroundColor(0, 0, 0, 150)  // Черный для фона
	{
		// Настройка прямоугольников для отрисовки
		backgroundRect.setSize(sf::Vector2f(barWidth, barHeight));
		backgroundRect.setFillColor(backgroundColor);
		backgroundRect.setOutlineColor(sf::Color::White);
		backgroundRect.setOutlineThickness(1.0f);

		armorRect.setSize(sf::Vector2f(barWidth, barHeight));
		armorRect.setFillColor(armorColor);

		healthRect.setSize(sf::Vector2f(barWidth, barHeight));
		healthRect.setFillColor(healthColor);
	}

	HealthBarComponent::~HealthBarComponent()
	{
	}

	void HealthBarComponent::Update(float deltaTime)
	{
		if (healthComponent)
		{
			UpdateHealthBars();
		}
	}

	void HealthBarComponent::Render()
	{
		if (!healthComponent || healthComponent->IsDead())
			return;

		auto transform = gameObject->GetComponent<TransformComponent>();
		if (!transform)
			return;

		auto& window = RenderSystem::Instance()->GetMainWindow();
		auto camera = gameObject->GetComponent<CameraComponent>();
		
		// Для всех объектов (игрока и врагов) используем мировые координаты
		// Полоски здоровья следуют за объектом
		float worldX = transform->GetWorldPosition().x + offsetX;
		float worldY = transform->GetWorldPosition().y + offsetY;
		DrawHealthBar(window, worldX, worldY);
	}

	void HealthBarComponent::SetHealthBarSize(float width, float height)
	{
		barWidth = width;
		barHeight = height;
		
		backgroundRect.setSize(sf::Vector2f(barWidth, barHeight));
		armorRect.setSize(sf::Vector2f(barWidth, barHeight));
		healthRect.setSize(sf::Vector2f(barWidth, barHeight));
	}

	void HealthBarComponent::SetHealthBarOffset(float x, float y)
	{
		offsetX = x;
		offsetY = y;
	}

	void HealthBarComponent::SetHealthBarColors(const sf::Color& healthColor, const sf::Color& armorColor, const sf::Color& backgroundColor)
	{
		this->healthColor = healthColor;
		this->armorColor = armorColor;
		this->backgroundColor = backgroundColor;
		
		backgroundRect.setFillColor(backgroundColor);
		armorRect.setFillColor(armorColor);
		healthRect.setFillColor(healthColor);
	}

	void HealthBarComponent::SetHealthComponent(HealthComponent* healthComponent)
	{
		this->healthComponent = healthComponent;
	}

	void HealthBarComponent::UpdateHealthBars()
	{
		if (!healthComponent)
			return;

		// Обновляем размеры полосок в зависимости от текущего состояния здоровья и брони
		float healthPercentage = healthComponent->GetHealthPercentage();
		float armorPercentage = healthComponent->GetArmorPercentage();

		// Полоска здоровья
		float healthWidth = barWidth * healthPercentage;
		healthRect.setSize(sf::Vector2f(healthWidth, barHeight));

		// Полоска брони (отображается поверх здоровья)
		float armorWidth = barWidth * armorPercentage;
		armorRect.setSize(sf::Vector2f(armorWidth, barHeight));
	}

	void HealthBarComponent::DrawHealthBar(sf::RenderWindow& window, float x, float y)
	{
		// Устанавливаем позицию фона
		backgroundRect.setPosition(x - barWidth / 2.0f, y);

		// Устанавливаем позицию полоски здоровья
		healthRect.setPosition(x - barWidth / 2.0f, y);

		// Устанавливаем позицию полоски брони (поверх здоровья)
		armorRect.setPosition(x - barWidth / 2.0f, y);

		// Отрисовываем в порядке: фон, здоровье, броня
		window.draw(backgroundRect);
		window.draw(healthRect);
		window.draw(armorRect);
	}
}
