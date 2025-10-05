#include "Floor.h"
#include <ResourceSystem.h>
#include <GameWorld.h>
#include <SpriteRendererComponent.h>
#include <SpriteColliderComponent.h>
#include <RigidbodyComponent.h>
#include <Logger.h>

using namespace XYZEngine;

namespace XYZRoguelike
{
	Floor::Floor(float x, float y, TileType tileType)
		: tileType(tileType), decorativeGameObject(nullptr)
	{
		// Создаем базовый слой
		CreateBaseLayer(x, y);
		
		// Создаем декоративный слой если нужно
		CreateDecorativeLayer(x, y);
	}

	Floor::~Floor()
	{
		// GameObject'ы будут удалены автоматически GameWorld'ом
		// Здесь мы только обнуляем указатели
		gameObject = nullptr;
		decorativeGameObject = nullptr;
	}

	void Floor::CreateBaseLayer(float x, float y)
	{
		gameObject = GameWorld::Instance()->CreateGameObject("Floor");
		
		auto floorRenderer = gameObject->AddComponent<SpriteRendererComponent>();
		if (floorRenderer)
		{
			// Используем простую текстуру platform и зеленый цвет для всех тайлов
			floorRenderer->SetTexture(*ResourceSystem::Instance()->GetTextureShared("platform"));
			floorRenderer->SetPixelSize(128, 128); // Увеличиваем размер тайлов в 2 раза: 64 -> 128
			floorRenderer->SetColor(sf::Color::Green); // Все тайлы пола зеленые
		}

		auto transform = gameObject->GetComponent<TransformComponent>();
		if (transform)
		{
			transform->SetLocalPosition(x, y);
		}

		// Добавляем физику только для непроходимых тайлов
		if (TileNeedsCollision(tileType))
		{
			auto body = gameObject->AddComponent<RigidbodyComponent>();
			body->SetKinematic(true); // Непроходимые тайлы должны быть кинематичными

			auto collider = gameObject->AddComponent<SpriteColliderComponent>();
			LOG_DEBUG("Added collision to tile: " + std::string(GetBaseTileTextureName(tileType)), "FLOOR");
		}
	}

	void Floor::CreateDecorativeLayer(float x, float y)
	{
		// Убираем декоративные элементы - оставляем только зеленые тайлы
		// decorativeGameObject остается nullptr
	}

	XYZEngine::GameObject* Floor::GetGameObject()
	{
		return gameObject;
	}

	TileType Floor::GetTileType() const
	{
		return tileType;
	}
}
