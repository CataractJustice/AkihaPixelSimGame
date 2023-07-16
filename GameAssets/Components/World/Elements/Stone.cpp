#include "Stone.h"
#include "Math/randrange.h"

Stone::Stone() 
{
	color = glm::vec4(0.2f, 0.2f, 0.21f, 1.0f) + glm::vec4(randrange(-0.05f, 0.05f), randrange(-0.05f, 0.05f), randrange(-0.05f, 0.05f), 0.0f);
	mass = 2.0f;
	rigidity = 1.0f;
}

void Stone::particleBehaviour(Particle& particle) 
{
	sandParticleBehaviour(this, particle, 0.02f);
}

void Stone::updateBehaviour(glm::vec2 at, const Particle& by) 
{
	fallingActivityUpdateBehaviour(this, at, by, 5);
}