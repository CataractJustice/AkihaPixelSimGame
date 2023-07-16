#pragma once
#include "Particle.h"

//640*480 8*8

class Element
{
	private:
	public:
	glm::vec4 color;
	float mass;
	float rigidity;
	float temperature;
	float thermalConductivity;
	Element();
	WorldElement* worldElement;
	virtual void particleBehaviour(Particle& particle);
	virtual void updateBehaviour(glm::vec2 at, const Particle& by);
};