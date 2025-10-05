#pragma once

#include "pch.h"
#include "Component.h"
#include <SFML/Graphics.hpp>

namespace XYZEngine
{
	class HealthComponent;

	class HealthBarComponent : public Component
	{
	public:
		HealthBarComponent(GameObject* gameObject);
		virtual ~HealthBarComponent();

		virtual void Update(float deltaTime) override;
		virtual void Render() override;

		// Настройка внешнего вида
		void SetHealthBarSize(float width, float height);
		void SetHealthBarOffset(float x, float y);
		void SetHealthBarColors(const sf::Color& healthColor, const sf::Color& armorColor, const sf::Color& backgroundColor);

		// Связывание с HealthComponent
		void SetHealthComponent(HealthComponent* healthComponent);

	private:
		void UpdateHealthBars();
		void DrawHealthBar(sf::RenderWindow& window, float x, float y);

		HealthComponent* healthComponent;
		
		// Размеры и позиция
		float barWidth;
		float barHeight;
		float offsetX;
		float offsetY;

		// Цвета
		sf::Color healthColor;
		sf::Color armorColor;
		sf::Color backgroundColor;

		// SFML объекты для отрисовки
		sf::RectangleShape backgroundRect;
		sf::RectangleShape armorRect;
		sf::RectangleShape healthRect;
	};
}
