#include "ElementEraser.h"
#include "GameNode/ComponentFactory.h"
#include "System/Input.h"
#include "Math/randrange.h"
#include "Components/World/Particle.h"

ElementEraser::ElementEraser() 
{
	addProp(new FloatProp(&radius), "Erase radius");
	this->radius = 8.0f;
}

void ElementEraser::update() 
{
	glm::vec2 erasePos = this->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
	glm::vec2 drawDir = glm::normalize(Input::Mouse::getMousePos() - (Graphics::window->getWindowResolution() / 2.0f));
	for(float i = 0.0f; i < 32.0f; i += 1.0f) 
	{
		erasePos += drawDir;
		WorldElement& element = Particle::world->getElement(erasePos);
		if(element.element) 
		{
			break;
		}
	}

	if(Input::Mouse::isButtonPressed(2)) 
	{	
		for(int i = -radius; i <= radius; i++) 
		{
			for(int j = -radius; j <= radius; j++) 
			{
				if((i*i+j*j) > radius * radius) 
				{
					if(j > 0) break;
					continue;
				};
				if(Particle::world->getElement(glm::ivec2(erasePos) + glm::ivec2(j, i)).element)
					Particle::world->eraseElement(glm::ivec2(erasePos) + glm::ivec2(j, i));
			}
		}
	}
}

EXPORT_COMPONENT(ElementEraser);