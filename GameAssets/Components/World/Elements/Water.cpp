#include "Water.h"
#include "Math/randrange.h"
#include <random>

Water::Water() 
{
	color = glm::vec4(0.2f, 0.5f, 0.9f, 0.5f) + glm::vec4(0.0f, randrange(0.00f, 0.2f), randrange(-0.05f, 0.05f), 0.0f);
	mass = 0.8f;
	rigidity = 0.005f;
}

void Water::particleBehaviour(Particle& particle) 
{
	WorldElement collidedWith;
	//this should never hault unless particle was already located at the same space as another particle or it is out of bounds which should not happen normally
	while(!particle.move(particle.Data().position + particle.Data().velocity, collidedWith) && (glm::abs(particle.Data().velocity.x + particle.Data().velocity.y) > 0.0001f)) {
		particle.Data().velocity *= 0.6f; 
		if(collidedWith.particleId) 
		{
			Particle::getById(collidedWith.particleId).Data().velocity += particle.Data().velocity * 0.4f;
		}
	};

	WorldElement& dEl = Particle::world->getElement(particle.Data().position + glm::vec2(0.0f, -1.0f));

	int flow = 0;
	int lflow = 0;
	int rflow = 0;

	for(int i = 0; i <= 12; i++) 
	{
		WorldElement& luEl = Particle::world->getElement(particle.Data().position + glm::vec2(-i, 0.0f));
		if(luEl.element) break;
		WorldElement& lEl = Particle::world->getElement(particle.Data().position + glm::vec2(-i, -1.0f));
		if(lEl.element) break;
		lflow++;
	}
	for(int i = 0; i <= 12; i++) 
	{
		WorldElement& ruEl = Particle::world->getElement(particle.Data().position + glm::vec2(i, 0.0f));
		if(ruEl.element) break;
		WorldElement& rEl = Particle::world->getElement(particle.Data().position + glm::vec2(i, -1.0f));
		if(rEl.element) break;
		rflow++;
	}
	int pressure = 0;

	for(int i = 0; i <= 12; i++) 
	{
		WorldElement& uEl = Particle::world->getElement(particle.Data().position + glm::vec2(0.0f, i));
		if(uEl.element) 
		{
			pressure++;
		}
		else 
		{
			break;
		}
	}

	flow = (lflow > rflow) ? -lflow : rflow;

	flow = ((float)flow) * ((float)(pressure) / 8.0f) * ((float)(pressure) / 8.0f);

	if(dEl.element)
	{
		if(!flow) 
		{
			particle.Data().velocity.x += randrange(-0.2f, 0.2f);
		}
		else 
		{
			particle.Data().velocity.x += randrange(0.0f, (float)flow / 240.0f);
		}
	}
	else 
	{
		particle.Data().velocity -= glm::vec2(0.0f, Particle::world->gravity);
	}
}

void Water::updateBehaviour(glm::vec2 at, const Particle& by) 
{
	WorldElement& wel = Particle::world->getElement(at);
	WorldElement& rEl = Particle::world->getElement(at + glm::vec2(1.0f, 0.0f));
	WorldElement& lEl = Particle::world->getElement(at - glm::vec2(1.0f, 0.0f));
	WorldElement& uEl = Particle::world->getElement(at + glm::vec2(0.0f, 1.0f));
	WorldElement& dEl = Particle::world->getElement(at - glm::vec2(0.0f, 1.0f));
	
	if(
		(rEl.element || &rEl == &World::boundsElement) &&
		(lEl.element || &lEl == &World::boundsElement) &&
		(uEl.element || &uEl == &World::boundsElement) &&
		(dEl.element || &dEl == &World::boundsElement)
	)
	{
		if(wel.particleId) 
		{
			Particle::world->stopParticle(at);
		}
		return;
	}
	

	if(wel.particleId == 0)
	{
		Particle::world->makeParticle(at);
		WorldElement collidedWith;
		if(wel.particleId) 
		{
			if(by.getId() && (int)(by.Data().position.y) < at.y) 
			{
				Particle::getById(wel.particleId).Data().velocity.y = by.Data().velocity.y - Particle::world->gravity;
			}
			if(!(uEl.element || &uEl == &World::boundsElement))
			Particle::getById(wel.particleId).Data().velocity.x = by.Data().velocity.x + glm::sign(by.Data().velocity.x) * Particle::world->gravity;
		}
	}
}