#include "Particle.h"
#include "Element.h"
#include <thread>
#include "Math/randrange.h"

ParticleData Particle::particles[ParticleLimit];
ParticleData& Particle::nullParticle = Particle::particles[0];
World* Particle::world = nullptr;
unsigned int Particle::freeParticleSlotsCount = 0;
unsigned int Particle::freeParticleSlots[ParticleLimit];

Particle::Particle(unsigned int id) : id(id) 
{
	
}

void Particle::update() 
{
	Data().element->particleBehaviour(*this);
}

bool Particle::move(glm::vec2 to, WorldElement& collidedWith) 
{
	if(glm::floor(to) == glm::floor(Data().position)) 
	{
		Data().position = to;
		return true;
	}
	
	WorldElement& nextElement = Particle::world->getElement(to);
	WorldElement& thisElement = *Data().element->worldElement;

	const float diffuseMassThreshold = 0.001f;

	//
	if(
		(nextElement.element && ((thisElement.element->mass - nextElement.element->mass) < diffuseMassThreshold)) ||
		(&nextElement == &World::boundsElement)
	) 
	{
		collidedWith = nextElement;
		return false;
	}

	if(nextElement.element && randrange(0.0f, 1.0f) < nextElement.element->rigidity) 
	{
		collidedWith = nextElement;
		return false;
	}

	if(thisElement.element != Data().element) 
	{
		glm::vec2 p = Data().position;
		glm::vec2 v = Data().velocity;
		throw "Invalid element";
	}

	//
	thisElement.particleId = nextElement.particleId;
	nextElement.particleId = id;

	thisElement.element = nextElement.element;
	nextElement.element = Data().element;
	
	if(thisElement.element) 
	{
		thisElement.element->worldElement = &thisElement;
		if(thisElement.particleId) Particle::getById(thisElement.particleId).Data().position = Data().position;
	}
	Data().element->worldElement = &nextElement;

	world->setPixelColor(Data().position, thisElement.element ? thisElement.element->color : glm::vec4(0.0f));
	world->setPixelColor(to, Data().element->color);

	glm::vec2 relativeUpdate[] = {glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(0.0f, 1.0f)};
	for(int i = 0; i < 4; i++) 
	{

		glm::vec2 updateIndexFrom = Data().position + relativeUpdate[i];
		glm::vec2 updateIndexTo = to + relativeUpdate[i];
		
		Element* el = Particle::world->getElement(updateIndexFrom).element;
		if(el) el->updateBehaviour(updateIndexFrom, *this);

		el = Particle::world->getElement(updateIndexTo).element;
		if(el) el->updateBehaviour(updateIndexTo, *this);
	}


	//
	if(thisElement.element) thisElement.element->updateBehaviour(Data().position, Particle::getById(0));
	Data().position = to;
	nextElement.element->updateBehaviour(to, Particle::getById(0));

	return true;
}

Particle Particle::getById(unsigned int id) 
{
	return Particle(id);
}
Particle Particle::create(glm::vec2 at, Element* element) 
{
	if(element == nullptr) 
	{
		throw "element is null.";
	}
	if(Particle::freeParticleSlotsCount == 0) return Particle(0);

	unsigned int newId = Particle::freeParticleSlots[--Particle::freeParticleSlotsCount];

	ParticleData& data = particles[newId];
	data.active = true;
	data.element = element;
	data.position = at;
	data.velocity = glm::vec2(0.0f, 0.0f);
	return Particle(newId);
}
void Particle::updateAll() 
{
	Particle::updateRange(1, ParticleLimit);
}

void Particle::updateRange(int begin, int end) 
{
	for(int i = begin; i < end; i++) 
	{
		ParticleData& particle = particles[i];
		if(particle.active) 
		{
			Particle(i).update();
		}
	}
}

void Particle::stopParticle() 
{
	Data().element = nullptr;
	Data().position = glm::vec2(0.0f);
	Data().velocity = glm::vec2(0.0f);
	Data().active = false;
	Particle::freeParticleSlots[freeParticleSlotsCount++] = id;
}

void Particle::stopAll() 
{
	freeParticleSlotsCount = 0;
	for(int i = 1; i < ParticleLimit; i++) 
	{
		Particle(i).stopParticle();
	}
}