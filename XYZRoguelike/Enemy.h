#pragma once

#include <GameObject.h>

namespace XYZRoguelike
{
	class Enemy
	{
	public:
		Enemy();
		XYZEngine::GameObject* GetGameObject();
		void SetPlayer(XYZEngine::GameObject* player);

	private:
		XYZEngine::GameObject* gameObject;
	};
}
