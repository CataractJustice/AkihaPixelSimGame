#pragma once
#include "Components/World/Element.h"

class Water : public Element
{
private:
public:
	Water();
	virtual void particleBehaviour(Particle& particle) override;
	virtual void updateBehaviour(glm::vec2 at, const Particle& by) override;
};