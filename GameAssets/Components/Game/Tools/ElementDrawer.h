#pragma once
#include "Components/World/World.h"

class ElementDrawer : public Component 
{
	private:
	float radius;
	public:
	ElementDrawer();
	void update() override;
};