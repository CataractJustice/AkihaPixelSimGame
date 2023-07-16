#include "Soil.h"
#include "Math/randrange.h"
#include "CommonBehaviours.h"
#include <random>

Soil::Soil() 
{
	color = glm::vec4(0.2f, 0.15f, 0.03f, 1.0f) + glm::vec4(randrange(-0.01f, 0.01f), randrange(-0.01f, 0.01f), randrange(-0.01f, 0.01f), 0.0f);
	mass = 1.0f;
	rigidity = 0.7f;
}

void Soil::particleBehaviour(Particle& particle) 
{
	sandParticleBehaviour(this, particle, 0.04f);
}

void Soil::updateBehaviour(glm::vec2 at, const Particle& by) 
{
	fallingActivityUpdateBehaviour(this, at, by, 2);
}