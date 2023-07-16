#pragma once
#include "GameNode/Component.h"
#include <vector>
#include "Components/World/World.h"
using namespace TealEngine;

class WorldCollidable : public Component
{
	public:
	virtual void onWorldCollision(const std::vector<std::pair<glm::ivec2, WorldElement*>>& elements) = 0;
};