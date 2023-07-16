#pragma once
#include "WorldCollidable.h"

using namespace TealEngine;

class GameBody : public WorldCollidable
{
	private:
	
	public:
	glm::vec2 velocity;
	void onWorldCollision(const std::vector<std::pair<glm::ivec2, WorldElement*>>& elements) override;
	void update() override;
};