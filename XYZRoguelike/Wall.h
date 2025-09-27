#pragma once

#include <GameObject.h>

namespace XYZRoguelike
{
	class Wall
	{
	public:
		Wall(float x, float y);
		XYZEngine::GameObject* GetGameObject();

	private:
		XYZEngine::GameObject* gameObject;
	};
}
