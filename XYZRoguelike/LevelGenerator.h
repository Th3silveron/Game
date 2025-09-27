#pragma once

#include <vector>
#include "Wall.h"
#include "Floor.h"

namespace XYZRoguelike
{
	class LevelGenerator
	{
	public:
		LevelGenerator();
		~LevelGenerator();

		void GenerateLevel(int size, float tileSize = 32.0f);
		void ClearLevel();

	private:
		std::vector<Wall*> walls;
		std::vector<Floor*> floors;

		void CreateWalls(int size, float tileSize);
		void CreateFloor(int size, float tileSize);
		bool IsWallPosition(int x, int y, int size);
	};
}
