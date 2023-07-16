#include "ElementDrawer.h"
#include "GameNode/ComponentFactory.h"
#include "System/Input.h"
#include "Components/World/Elements/Sand.h"
#include "Math/randrange.h"

ElementDrawer::ElementDrawer() 
{
	addProp(new FloatProp(&radius), "Draw radius");
	this->radius = 8.0f;
}

void ElementDrawer::update() 
{
	glm::vec2 drawPos = this->getParentOfType<GameNode3D>()->getWorldTransform().getPosition();
	glm::vec2 drawDir = glm::normalize(Input::Mouse::getMousePos() - (Graphics::window->getWindowResolution() / 2.0f));
	for(float i = 0.0f; i < 32.0f; i += 1.0f) 
	{
		drawPos += drawDir;
		WorldElement& element = Particle::world->getElement(drawPos);
		if(element.element) 
		{
			break;
		}
	}

	if(Input::Mouse::isButtonPressed(0)) 
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
				Element* sand = new Sand();
				Particle::world->putElement(glm::ivec2(drawPos) + glm::ivec2(j, i), sand, true);
			}
		}
	}
}

EXPORT_COMPONENT(ElementDrawer);