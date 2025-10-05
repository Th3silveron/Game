#pragma once

namespace XYZRoguelike
{
    enum class TileType
    {
        Grass,      // Трава - проходимая (единственный тип пола)
        Count
    };

    // Получить имя текстуры для базового слоя тайла
    inline const char* GetBaseTileTextureName(TileType type)
    {
        // Все тайлы используют простую текстуру platform
        return "platform";
    }

    // Получить имя текстуры для декоративного слоя тайла
    inline const char* GetDecorativeTileTextureName(TileType type)
    {
        // Убираем декоративные элементы - всегда возвращаем nullptr
        return nullptr;
    }

    // Получить имя текстуры для типа тайла (обратная совместимость)
    inline const char* GetTileTextureName(TileType type)
    {
        return GetBaseTileTextureName(type);
    }

    // Проверить, проходим ли тайл
    inline bool IsTileWalkable(TileType type)
    {
        // Только трава - всегда проходима
        return type == TileType::Grass;
    }

    // Проверить, нужна ли коллизия для тайла
    inline bool TileNeedsCollision(TileType type)
    {
        return !IsTileWalkable(type);
    }
}
