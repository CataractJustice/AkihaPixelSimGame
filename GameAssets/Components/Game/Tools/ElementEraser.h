#pragma once
#include "Components/World/World.h"

class ElementEraser : public Component 
{
	private:
	float radius;
	public:
	ElementEraser();
	void update() override;
};