#include "GameBody.h"
#include "GameNode/ComponentFactory.h"

void GameBody::onWorldCollision(const std::vector<std::pair<glm::ivec2, WorldElement*>>& elements) 
{
	Transform& transform = this->getParentOfType<GameNode3D>()->getRelativeTransform();
	glm::ivec3 ipos = this->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
	for(const auto& elPair : elements) 
	{
		if(elPair.first.x > ipos.x) 
		{
			transform.translate(glm::vec3(-1.0f / elements.size(), 0.0f, 0.0f));
			if(velocity.x > 0.0f) velocity.x -= 0.1f;
		}
		if(elPair.first.x < ipos.x) 
		{
			transform.translate(glm::vec3(1.0f / elements.size(), 0.0f, 0.0f));
			if(velocity.x < 0.0f) velocity.x += 0.1f;
		}
		if(elPair.first.y > ipos.y) 
		{
			transform.translate(glm::vec3(0.0f, -1.0f / elements.size(), 0.0f));
			if(velocity.y > 0.0f) velocity.y -= 0.1f;
		}
		if(elPair.first.y < ipos.y) 
		{
			transform.translate(glm::vec3(0.0f, 1.0f / elements.size(), 0.0f));
			if(velocity.y < 0.0f) velocity.y += 0.1f;
		}
	}
	velocity *= 0.98f;
}

void GameBody::update() 
{
	Transform& transform = this->getParentOfType<GameNode3D>()->getRelativeTransform();
	transform.translate(glm::vec3(velocity.x, velocity.y, 0.0f));
	this->velocity += glm::vec2(0.0f, -0.1f);
}

EXPORT_COMPONENT(GameBody);