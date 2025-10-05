#include "LevelGenerator.h"
#include <Logger.h>

using namespace XYZEngine;

namespace XYZRoguelike
{
	LevelGenerator::LevelGenerator()
		: randomGenerator(std::random_device{}())
	{
		LOG_DEBUG("LevelGenerator initialized", "LEVEL_GENERATOR");
	}

	LevelGenerator::~LevelGenerator()
	{
		ClearLevel();
	}

	void LevelGenerator::GenerateLevel(int size, float tileSize)
	{
		LOG_INFO("Generating level with size: " + std::to_string(size) + "x" + std::to_string(size), "LEVEL_GENERATOR");
		ClearLevel();
		CreateWalls(size, tileSize);
		CreateFloor(size, tileSize);
		LOG_INFO("Level generation completed", "LEVEL_GENERATOR");
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
		LOG_DEBUG("Creating walls...", "LEVEL_GENERATOR");
		int wallCount = 0;
		
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
					wallCount++;
				}
			}
		}
		
		LOG_INFO("Created " + std::to_string(wallCount) + " walls", "LEVEL_GENERATOR");
	}

	void LevelGenerator::CreateFloor(int size, float tileSize)
	{
		LOG_DEBUG("Creating floor tiles...", "LEVEL_GENERATOR");
		int tileCount = 0;
		
		// Создаем только зеленые тайлы травы
		for (int x = 1; x < size - 1; x++)
		{
			for (int y = 1; y < size - 1; y++)
			{
				float worldX = x * tileSize;
				float worldY = y * tileSize;
				floors.push_back(new Floor(worldX, worldY, TileType::Grass));
				tileCount++;
			}
		}
		
		LOG_INFO("Created " + std::to_string(tileCount) + " green floor tiles", "LEVEL_GENERATOR");
	}

	bool LevelGenerator::IsWallPosition(int x, int y, int size)
	{
		// Стены по периметру
		return x == 0 || x == size - 1 || y == 0 || y == size - 1;
	}
}