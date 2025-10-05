#pragma once

#include <vector>
#include <random>
#include "Wall.h"
#include "Floor.h"
#include "TileTypes.h"

namespace XYZRoguelike
{
	class LevelGenerator
	{
	public:
		LevelGenerator();
		~LevelGenerator();

		void GenerateLevel(int size, float tileSize = 128.0f);
		void ClearLevel();

	private:
		std::vector<Wall*> walls;
		std::vector<Floor*> floors;
		std::mt19937 randomGenerator;

		void CreateWalls(int size, float tileSize);
		void CreateFloor(int size, float tileSize);
		bool IsWallPosition(int x, int y, int size);
	};
}
