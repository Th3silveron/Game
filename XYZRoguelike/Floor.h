#pragma once

#include <GameObject.h>
#include "TileTypes.h"

namespace XYZRoguelike
{
	class Floor
	{
	public:
		Floor(float x, float y, TileType tileType = TileType::Grass);
		~Floor();
		XYZEngine::GameObject* GetGameObject();
		TileType GetTileType() const;

	private:
		XYZEngine::GameObject* gameObject;
		XYZEngine::GameObject* decorativeGameObject; // Для декоративного слоя (деревья)
		TileType tileType;
		
		void CreateBaseLayer(float x, float y);
		void CreateDecorativeLayer(float x, float y);
	};
}
