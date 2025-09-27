#include "LevelGenerator.h"

namespace XYZRoguelike
{
	LevelGenerator::LevelGenerator()
	{
	}

	LevelGenerator::~LevelGenerator()
	{
		ClearLevel();
	}

	void LevelGenerator::GenerateLevel(int size, float tileSize)
	{
		ClearLevel();
		CreateWalls(size, tileSize);
		CreateFloor(size, tileSize);
	}

	void LevelGenerator::ClearLevel()
	{
		// Очищаем стены
		for (auto wall : walls)
		{
			delete wall;
		}
		walls.clear();

		// Очищаем пол
		for (auto floor : floors)
		{
			delete floor;
		}
		floors.clear();
	}

	void LevelGenerator::CreateWalls(int size, float tileSize)
	{
		// Создаем стены по периметру
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				if (IsWallPosition(x, y, size))
				{
					float worldX = x * tileSize;
					float worldY = y * tileSize;
					walls.push_back(new Wall(worldX, worldY));
				}
			}
		}
	}

	void LevelGenerator::CreateFloor(int size, float tileSize)
	{
		// Заполняем внутреннюю область полом
		for (int x = 1; x < size - 1; x++)
		{
			for (int y = 1; y < size - 1; y++)
			{
				float worldX = x * tileSize;
				float worldY = y * tileSize;
				floors.push_back(new Floor(worldX, worldY));
			}
		}
	}

	bool LevelGenerator::IsWallPosition(int x, int y, int size)
	{
		// Стены по периметру
		return x == 0 || x == size - 1 || y == 0 || y == size - 1;
	}
}
