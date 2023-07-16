#include "PlayerController.h"
#include "System/Input.h"
#include "GameNode/ComponentFactory.h"
#include "Components/World/Particle.h"

PlayerController::PlayerController() 
{
	addProp(new IntProp(&feetOffset), "Feet offset");
	addProp(new BoolProp(&onGround), "On ground");
	addProp(new FloatProp(&jumpVelocity), "Jump velocity");
	addProp(new FloatProp(&moveAcceleration), "Move acceleration");
}

void PlayerController::update() 
{
	if(Input::Keyboard::isKeyPressed(GLFW_KEY_A)) 
	{
		this->body->velocity.x += this->moveAcceleration;
	}
	if(Input::Keyboard::isKeyPressed(GLFW_KEY_D)) 
	{
		this->body->velocity.x -= this->moveAcceleration;
	}
}

void PlayerController::onAttach() 
{
	this->body = this->getParent()->findComponentsByType<GameBody>()[0];
	Particle::world->chunkLoader = this->getParentOfType<GameNode3D>();
}

void PlayerController::onWorldCollision(const std::vector<std::pair<glm::ivec2, WorldElement*>>& elements) 
{
	onGround = false;
	for(auto element : elements) 
	{
		if(element.first.y < this->getParentOfType<GameNode3D>()->getWorldTransform().getPosition().y + feetOffset) onGround = true;
	}

	if(Input::Keyboard::isKeyPressed(GLFW_KEY_W)) 
	{
		this->body->velocity.y = this->jumpVelocity;
	}
}

EXPORT_COMPONENT(PlayerController);