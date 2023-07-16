#pragma once
#include "WorldCollidable.h"
#include "GameBody.h"

class PlayerController : public WorldCollidable
{
	private:
	int feetOffset;
	float jumpVelocity;
	float moveAcceleration;
	bool onGround;

	GameBody* body;

	public:
	PlayerController();
	void onAttach() override;
	void update() override;
	void onWorldCollision(const std::vector<std::pair<glm::ivec2, WorldElement*>>& elements) override;
};