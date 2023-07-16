#include "GameNode/Component.h"

using namespace TealEngine;

class CollisionBox : public Component
{
private:
	/* data */
public:
	glm::vec2 pxFrom;
	glm::vec2 pxTo;
	int tolerance;
	void update() override;
	CollisionBox();
};
