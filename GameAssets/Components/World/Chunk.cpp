#include "Chunk.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Core.h"
#include "Element.h"

const float airThermalConductivity = 0.1f;
const float ambientTemperature = 16.0f;

Chunk::Chunk(unsigned int width, unsigned int height) : width(width), height(height) 
{
	this->albedo = Texture(GL_TEXTURE_2D, GL_RGBA, GL_RGBA);
	this->albedo.create(width, height);
	this->elements = new WorldElement[width * height];
	this->albedoBuffer = new char[width * height * 4];
	this->thermalInfoBuffer = new float[width * height * 4];
	for(int i = 0; i < width * height; i++) 
	{
		if(elements[i].element) 
		{
			thermalInfoBuffer[i * 4 + 0] = ambientTemperature;
		}
	}
}

Chunk::~Chunk() 
{
	for(int i = 0; i < width * height; i++) 
	{
		if(elements[i].particleId) 
		{
			if(Particle::getById(elements[i].particleId).Data().active)
				Particle::getById(elements[i].particleId).stopParticle();
		}

		if(elements[i].element) delete elements[i].element;
	}
	delete[] this->albedoBuffer;
	delete[] this->elements;
}

WorldElement& Chunk::getElement(glm::ivec2 at) 
{
	if((at.x + at.y * this->width) >= (640 * 480) || ((at.x + at.y * this->width) < 0)) 
	{
		throw "Out of bounds.";
	}
	return elements[at.x + at.y * this->width];
}

void Chunk::onAttach() 
{
	this->rename("Chunk");
	mesh = new Mesh3DRenderer();
	material = Core::materialsManager.get("Default");
	material.setTexture("textureImage", this->albedo.id());
	mesh->setMesh(BasicMeshes::square.sharedMesh());
	mesh->setShader(&material);
	getParent()->attachComponent(mesh);
}

void Chunk::updateParticles() 
{
	for(int i = 0; i < width * height; i++) 
	{
		if(elements[i].particleId) 
		{
			if(Particle::getById(elements[i].particleId).Data().active)
				Particle::getById(elements[i].particleId).update();
		}
	}
}

void Chunk::setPixelColor(glm::ivec2 at, glm::ivec4 color) 
{
	glBindTexture(GL_TEXTURE_2D, (GLuint)this->albedo.id());
	char pixel[] = {(char)color.x, (char)color.y, (char)color.z, (char)color.w};
	glTexSubImage2D(GL_TEXTURE_2D, 0, at.x, at.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
}

void Chunk::updateDisplay() 
{
	for(int i = 0; i < width * height; i++) 
	{
		if(elements[i].element) 
		{
			albedoBuffer[i * 4 + 0] = elements[i].element->color.r * 255;
			albedoBuffer[i * 4 + 1] = elements[i].element->color.g * 255;
			albedoBuffer[i * 4 + 2] = elements[i].element->color.b * 255;
			albedoBuffer[i * 4 + 3] = elements[i].element->color.a * 255;

			thermalInfoBuffer[i * 4 + 0] = elements[i].element->temperature;
			thermalInfoBuffer[i * 4 + 1] = elements[i].element->thermalConductivity;
		}
		else 
		{
			albedoBuffer[i * 4 + 0] = 0;
			albedoBuffer[i * 4 + 1] = 0;
			albedoBuffer[i * 4 + 2] = 0;
			albedoBuffer[i * 4 + 3] = 0;
			thermalInfoBuffer[i * 4 + 1] = airThermalConductivity;
		}
	}

	albedo.create(width, height, albedoBuffer);
	thermal.create(width, height, thermalInfoBuffer);
}

EXPORT_COMPONENT(Chunk);