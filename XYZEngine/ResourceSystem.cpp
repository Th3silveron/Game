#include "pch.h"
#include "ResourceSystem.h"
#include "Logger.h"

namespace XYZEngine
{
	ResourceSystem* ResourceSystem::Instance()
	{
		static ResourceSystem resourceSystem;
		return &resourceSystem;
	}

	void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth)
	{
		ASSERT_LOG(!name.empty(), "Texture name cannot be empty", "RESOURCE");
		ASSERT_LOG(!sourcePath.empty(), "Texture source path cannot be empty", "RESOURCE");
		
		if (textures.find(name) != textures.end())
		{
			LOG_WARNING("Texture '" + name + "' already loaded, skipping", "RESOURCE");
			return;
		}

		LOG_INFO("Loading texture: '" + name + "' from '" + sourcePath + "'", "RESOURCE");
		
		sf::Texture* newTexture = new sf::Texture();
		if (newTexture->loadFromFile(sourcePath))
		{
			newTexture->setSmooth(isSmooth);
			textures.emplace(name, newTexture);
			LOG_INFO("Successfully loaded texture: '" + name + "'", "RESOURCE");
		}
		else
		{
			LOG_ERROR("Failed to load texture: '" + name + "' from '" + sourcePath + "'", "RESOURCE");
			delete newTexture;
		}
	}
	const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
	{
		ASSERT_LOG(!name.empty(), "Texture name cannot be empty", "RESOURCE");
		
		auto it = textures.find(name);
		if (it == textures.end())
		{
			LOG_ERROR("Texture '" + name + "' not found", "RESOURCE");
			return nullptr;
		}
		
		LOG_DEBUG("Retrieved shared texture: '" + name + "'", "RESOURCE");
		return it->second;
	}
	sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
	{
		return new sf::Texture(*textures.find(name)->second);
	}
	void ResourceSystem::DeleteSharedTexture(const std::string& name)
	{
		auto texturePair = textures.find(name);

		sf::Texture* deletingTexure = texturePair->second;
		textures.erase(texturePair);
		delete deletingTexure;
	}

	void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth)
	{
		if (textureMaps.find(name) != textureMaps.end())
		{
			return;
		}

		sf::Texture textureMap;
		if (textureMap.loadFromFile(sourcePath))
		{
			auto textureMapElements = new std::vector<sf::Texture*>();

			auto textureSize = textureMap.getSize();
			int loadedElements = 0;

			for (unsigned int y = 0; y <= textureSize.y - elementPixelSize.y; y += elementPixelSize.y)
			{
				if (loadedElements == totalElements)
				{
					break;
				}

				for (unsigned int x = 0; x <= textureSize.x - elementPixelSize.x; x += elementPixelSize.x)
				{
					if (loadedElements == totalElements)
					{
						break;
					}

					sf::Texture* newTextureMapElement = new sf::Texture();
					if (newTextureMapElement->loadFromFile(sourcePath, sf::IntRect(x, y, elementPixelSize.x, elementPixelSize.y)))
					{
						newTextureMapElement->setSmooth(isSmooth);
						textureMapElements->push_back(newTextureMapElement);
					}
					loadedElements++;
				}
			}

			textureMaps.emplace(name, *textureMapElements);
		}
	}
	const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const
	{
		auto textureMap = textureMaps.find(name);
		auto textures = textureMap->second;
		return textures[elementIndex];
	}
	sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const
	{
		auto textureMap = textureMaps.find(name);
		auto textures = textureMap->second;
		return new sf::Texture(*textures[elementIndex]);
	}
	int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
	{
		auto textureMap = textureMaps.find(name);
		auto textures = textureMap->second;
		return static_cast<int>(textures.size());
	}
	void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
	{
		auto textureMap = textureMaps.find(name);
		auto deletingTextures = textureMap->second;

		for (int i = 0; i < deletingTextures.size(); i++)
		{
			delete deletingTextures[i];
		}

		textureMaps.erase(textureMap);
	}

	void ResourceSystem::Clear()
	{
		DeleteAllTextures();
		DeleteAllTextureMaps();
		DeleteAllSoundBuffers();
		DeleteAllMusic();
	}

	void ResourceSystem::LoadSoundBuffer(const std::string& name, std::string sourcePath)
	{
		if (soundBuffers.find(name) != soundBuffers.end())
		{
			return;
		}

		sf::SoundBuffer* newSoundBuffer = new sf::SoundBuffer();
		if (newSoundBuffer->loadFromFile(sourcePath))
		{
			soundBuffers.emplace(name, newSoundBuffer);
		}
	}

	const sf::SoundBuffer* ResourceSystem::GetSoundBufferShared(const std::string& name) const
	{
		auto it = soundBuffers.find(name);
		if (it != soundBuffers.end())
		{
			return it->second;
		}
		return nullptr;
	}

	sf::SoundBuffer* ResourceSystem::GetSoundBufferCopy(const std::string& name) const
	{
		auto it = soundBuffers.find(name);
		if (it != soundBuffers.end())
		{
			return new sf::SoundBuffer(*it->second);
		}
		return nullptr;
	}

	void ResourceSystem::DeleteSharedSoundBuffer(const std::string& name)
	{
		auto soundBufferPair = soundBuffers.find(name);
		if (soundBufferPair != soundBuffers.end())
		{
			sf::SoundBuffer* deletingSoundBuffer = soundBufferPair->second;
			soundBuffers.erase(soundBufferPair);
			delete deletingSoundBuffer;
		}
	}

	void ResourceSystem::LoadMusic(const std::string& name, std::string sourcePath)
	{
		if (music.find(name) != music.end())
		{
			return;
		}

		sf::Music* newMusic = new sf::Music();
		if (newMusic->openFromFile(sourcePath))
		{
			music.emplace(name, newMusic);
		}
	}

	sf::Music* ResourceSystem::GetMusic(const std::string& name) const
	{
		auto it = music.find(name);
		if (it != music.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void ResourceSystem::DeleteMusic(const std::string& name)
	{
		auto musicPair = music.find(name);
		if (musicPair != music.end())
		{
			sf::Music* deletingMusic = musicPair->second;
			music.erase(musicPair);
			delete deletingMusic;
		}
	}

	void ResourceSystem::DeleteAllTextures()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& texturePair : textures)
		{
			keysToDelete.push_back(texturePair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTexture(key);
		}
	}
	void ResourceSystem::DeleteAllTextureMaps()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& textureMapPair : textureMaps)
		{
			keysToDelete.push_back(textureMapPair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTextureMap(key);
		}
	}

	void ResourceSystem::DeleteAllSoundBuffers()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& soundBufferPair : soundBuffers)
		{
			keysToDelete.push_back(soundBufferPair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedSoundBuffer(key);
		}
	}

	void ResourceSystem::DeleteAllMusic()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& musicPair : music)
		{
			keysToDelete.push_back(musicPair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteMusic(key);
		}
	}
}