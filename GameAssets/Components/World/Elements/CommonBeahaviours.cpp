#include "CommonBehaviours.h"
#include "../Element.h"
void fallingActivityUpdateBehaviour(Element* element, glm::vec2 at, const Particle& by, int updatesPerActivationChance) 
{
	
	WorldElement& wel = *(element->worldElement);
	WorldElement& rEl = Particle::world->getElement(at + glm::vec2(1.0f, 0.0f));
	WorldElement& lEl = Particle::world->getElement(at - glm::vec2(1.0f, 0.0f));
	WorldElement& uEl = Particle::world->getElement(at + glm::vec2(0.0f, 1.0f));
	WorldElement& dEl = Particle::world->getElement(at - glm::vec2(0.0f, 1.0f));
	
	if(
		(rEl.element || &rEl == &World::boundsElement) &&
		(lEl.element || &lEl == &World::boundsElement) &&
		(uEl.element || &uEl == &World::boundsElement) &&
		(dEl.element && (dEl.element->mass - element->mass) > 0.001f || &dEl == &World::boundsElement)
	)
	{
		if(wel.particleId) 
		{
			Particle::world->stopParticle(at);
		}
		return;
	}
	
	bool updatedBelove = by.Data().position.y < at.y && (glm::abs(at.y - by.Data().position.y) > glm::abs(at.y - by.Data().position.x));

	if(wel.particleId == 0 && (rand() % updatesPerActivationChance) == 0)
	{
		Particle::world->makeParticle(at);
		WorldElement collidedWith;
		if(wel.particleId) 
		{
			if(by.getId() && updatedBelove) 
			{
				Particle::getById(wel.particleId).Data().velocity.y = by.Data().velocity.y - Particle::world->gravity;
				//uEl.element->updateBehaviour(at + glm::vec2(0.0f, 1.0f), Particle::getById(wel.particleId));
			}
		}
	}
}

