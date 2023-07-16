#pragma once 
#include "Components/World/Element.h"
#include "CommonBehaviours.h"

class Stone : public Element 
{
	private:

	public:

	Stone();

	virtual void particleBehaviour(Particle& particle) override;
	virtual void updateBehaviour(glm::vec2 at, const Particle& by) override;
};