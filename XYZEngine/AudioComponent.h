#pragma once

#include "Component.h"
#include <SFML/Audio.hpp>

namespace XYZEngine
{
	class AudioComponent : public Component
	{
	public:
		AudioComponent(GameObject* gameObject);
		~AudioComponent();

		void Update(float deltaTime) override;
		void Render() override;

		void PlaySound(const std::string& soundBufferName);
		void StopSound();
		void SetSoundVolume(float volume);
		float GetSoundVolume() const;

		void SetLooping(bool looping);
		bool IsLooping() const;

		bool IsPlaying() const;

	private:
		sf::Sound sound;
		bool isLooping = false;
	};
}
