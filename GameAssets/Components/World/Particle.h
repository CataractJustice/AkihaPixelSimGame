#pragma once
#include "libs/glm/glm.hpp"
#include "World.h"
#define ParticleLimit (32000)
using namespace TealEngine;

class Element;

struct ParticleData 
{
	public:
	glm::vec2 position;
	glm::vec2 velocity;
	bool active;
	Element* element;
	inline ParticleData() : position(0.0f), velocity(0.0f), active(false) {};
};

class Particle 
{
	private:
	static ParticleData particles[ParticleLimit];
	static unsigned int freeParticleSlots[ParticleLimit];
	static unsigned int freeParticleSlotsCount;

	unsigned int id;

	Particle(unsigned int id);
	
	public:
	static ParticleData& nullParticle;

	static World* world;

	inline ParticleData& Data() const { return Particle::particles[this->id]; };

	void update();
	
	bool move(glm::vec2 to, WorldElement& collidedWith);

	inline unsigned int getId() const { return id; };
	void stopParticle();

	static Particle getById(unsigned int id);
	//will return null particle if particle limit is reached
	static Particle create(glm::vec2 at, Element* element);
	static void updateAll();
	static void updateRange(int begin, int end);
	static void stopAll();
};