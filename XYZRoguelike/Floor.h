#pragma once

#include <GameObject.h>

namespace XYZRoguelike
{
	class Floor
	{
	public:
		Floor(float x, float y);
		XYZEngine::GameObject* GetGameObject();

	private:
		XYZEngine::GameObject* gameObject;
	};
}
