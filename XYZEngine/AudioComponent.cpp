#include "pch.h"
#include "AudioComponent.h"
#include "ResourceSystem.h"

namespace XYZEngine
{
	AudioComponent::AudioComponent(GameObject* gameObject)
		: Component(gameObject)
	{
	}

	AudioComponent::~AudioComponent()
	{
		sound.stop();
	}

	void AudioComponent::Update(float deltaTime)
	{
		// Audio component doesn't need update logic
	}

	void AudioComponent::Render()
	{
		// Audio component doesn't need render logic
	}

	void AudioComponent::PlaySound(const std::string& soundBufferName)
	{
		const sf::SoundBuffer* soundBuffer = ResourceSystem::Instance()->GetSoundBufferShared(soundBufferName);
		if (soundBuffer)
		{
			sound.setBuffer(*soundBuffer);
			sound.setLoop(isLooping);
			sound.play();
		}
	}

	void AudioComponent::StopSound()
	{
		sound.stop();
	}

	void AudioComponent::SetSoundVolume(float volume)
	{
		sound.setVolume(volume);
	}

	float AudioComponent::GetSoundVolume() const
	{
		return sound.getVolume();
	}

	void AudioComponent::SetLooping(bool looping)
	{
		isLooping = looping;
		sound.setLoop(looping);
	}

	bool AudioComponent::IsLooping() const
	{
		return isLooping;
	}

	bool AudioComponent::IsPlaying() const
	{
		return sound.getStatus() == sf::Sound::Playing;
	}
}
