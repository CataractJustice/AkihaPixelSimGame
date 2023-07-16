#include "Sand.h"
#include "Math/randrange.h"
#include "CommonBehaviours.h"
#include <random>

Sand::Sand() 
{
	color = glm::vec4(1.0f, 0.7f, 0.5f, 1.0f) + glm::vec4(randrange(-0.2f, 0.00f), randrange(-0.1f, 0.1f), randrange(-0.1f, 0.1f), 0.0f);
	mass = 1.0f;
	rigidity = 0.5f;
}

void Sand::particleBehaviour(Particle& particle) 
{
	sandParticleBehaviour(this, particle, 0.04f);
}

void Sand::updateBehaviour(glm::vec2 at, const Particle& by) 
{
	fallingActivityUpdateBehaviour(this, at, by);
}

void sandParticleBehaviour(Element* element, Particle& particle, float spreadVelocity) 
{
	WorldElement collidedWith;
	//this should never hault unless particle was already located at the same space as another particle or it is out of bounds which should not happen normally
	while(!particle.move(particle.Data().position + particle.Data().velocity, collidedWith) && glm::abs(particle.Data().velocity.x + particle.Data().velocity.y) > 0.0001f) {
		particle.Data().velocity *= 0.5f; 
		if(collidedWith.particleId) 
		{
			Particle::getById(collidedWith.particleId).Data().velocity += particle.Data().velocity * 0.5f;
		}
	};

	WorldElement& rEl = Particle::world->getElement(particle.Data().position + glm::vec2(1.0f, -1.0f));
	WorldElement& dEl = Particle::world->getElement(particle.Data().position + glm::vec2(0.0f, -1.0f));
	WorldElement& lEl = Particle::world->getElement(particle.Data().position + glm::vec2(-1.0f, -1.0f));

	if(dEl.element)
	{
		if(!rEl.element && !lEl.element) 
		{
			particle.Data().velocity.x += randrange(-spreadVelocity, spreadVelocity);
		}
		else if(!rEl.element) 
		{
			particle.Data().velocity.x += spreadVelocity;
		}
		else if(!lEl.element) 
		{
			particle.Data().velocity.x -= spreadVelocity;
		}
	}
	if(!dEl.element || (dEl.element->mass - element->mass) > 0.001f) 
	{
		particle.Data().velocity -= glm::vec2(0.0f, Particle::world->gravity);
	}
}