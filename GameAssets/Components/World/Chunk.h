#pragma once
#include "GameNode/Component.h"
#include "GameNode/ComponentFactory.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include <stack>

using namespace TealEngine;

class Element;

struct WorldElement 
{
	public:
	unsigned int particleId;
	Element* element;

	inline WorldElement() : particleId(0), element(nullptr) {};
};

class Chunk : public Component
{
	private:
	
	Texture albedo;
	Texture thermal;
	unsigned int width, height;
	WorldElement* elements;
	char* albedoBuffer;
	float* thermalInfoBuffer;

	Mesh3DRenderer* mesh;
	ShaderProgram material;

	public:
	bool updateDisplayRequired;
	
	Chunk(unsigned int width = 640, unsigned int height = 480);
	~Chunk();

	void updateDisplay();

	void onAttach() override;

	void updateParticles();

	void setPixelInfo(glm::ivec2 at, glm::ivec4 color);

	WorldElement& getElement(glm::ivec2 at);
};