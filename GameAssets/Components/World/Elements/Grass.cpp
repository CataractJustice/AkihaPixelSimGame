#include "Grass.h"
#include "Math/randrange.h"
#include "CommonBehaviours.h"
#include <random>

Grass::Grass() 
{
	color = glm::vec4(0.2f, 0.8f, 0.04f, 1.0f) + glm::vec4(randrange(-0.01f, 0.01f), randrange(-0.01f, 0.01f), randrange(-0.01f, 0.01f), 0.0f);
	mass = 0.05f;
	rigidity = 0.001f;
}

void Grass::particleBehaviour(Particle& particle) 
{
	sandParticleBehaviour(this, particle, 0.002f);
}

void Grass::updateBehaviour(glm::vec2 at, const Particle& by) 
{
	fallingActivityUpdateBehaviour(this, at, by);
}