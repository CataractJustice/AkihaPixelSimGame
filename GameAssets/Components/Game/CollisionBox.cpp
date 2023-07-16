#include "CollisionBox.h"
#include "Core.h"
#include "Graphics/Renderer/Shapes/Line3D.h"
#include "GameNode/GameNode3D.h"
#include "GameNode/ComponentFactory.h"
#include "WorldCollidable.h"
#include "Components/World/World.h"
#include "Components/World/Particle.h"

CollisionBox::CollisionBox() 
{
	this->addProp(new FloatVecProp(glm::value_ptr(pxFrom), 2), "From");
	this->addProp(new FloatVecProp(glm::value_ptr(pxTo), 2), "To");
	this->addProp(new IntProp(&tolerance), "Tolerance");
}

void CollisionBox::update() 
{
	if(!Particle::world) return;
	Transform& transform = this->getParentOfType<GameNode3D>()->getWorldTransform();
	glm::vec3 pos = transform.getPosition();
	std::vector<std::pair<glm::ivec2, WorldElement*>> collidedWith;

	glm::ivec2 from = glm::ivec2(glm::vec2(pos.x, pos.y) + pxFrom);
	glm::ivec2 to = glm::ivec2(glm::vec2(pos.x, pos.y) + pxTo);

	if(this->getParent() == GameNode::getSelectedNode()) 
	{
		Core::shapesRenderer.pushShape(Line3D(glm::vec3(from.x, from.y, pos.z), glm::vec3(from.x, to.y, pos.z), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.01f));
		Core::shapesRenderer.pushShape(Line3D(glm::vec3(from.x, from.y, pos.z), glm::vec3(to.x, from.y, pos.z), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.01f));
		Core::shapesRenderer.pushShape(Line3D(glm::vec3(to.x, from.y, pos.z), glm::vec3(to.x, to.y, pos.z), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.01f));
		Core::shapesRenderer.pushShape(Line3D(glm::vec3(from.x, to.y, pos.z), glm::vec3(to.x, to.y, pos.z), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.01f));
	}
	for(int i = from.x; i <= to.x; i++) 
	{
		for(int j = from.y; j <= to.y; j++) 
		{
			WorldElement* wel = &Particle::world->getElement(glm::ivec2(i, j));
			if(wel->element) 
			{
				collidedWith.push_back(std::pair<glm::ivec2, WorldElement*>(glm::ivec2(i, j), wel));
			}
		}
	}

	if(collidedWith.size() > tolerance) 
	{
		std::vector<WorldCollidable*> collidables = this->getParent()->findComponentsByType<WorldCollidable>();
		for(WorldCollidable* wc : collidables) 
		{
			wc->onWorldCollision(collidedWith);
		}
	}
};

EXPORT_COMPONENT(CollisionBox);