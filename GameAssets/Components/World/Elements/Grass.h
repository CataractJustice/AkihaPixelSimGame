#pragma once
#include "Components/World/Element.h"

class Grass : public Element
{
private:

public:
	virtual void particleBehaviour(Particle& particle) override;
	virtual void updateBehaviour(glm::vec2 at, const Particle& by) override;
	Grass();
};