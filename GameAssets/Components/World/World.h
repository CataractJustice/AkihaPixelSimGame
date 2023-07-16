#pragma once
#include "GameNode/Component.h"
#include "GameNode/ComponentFactory.h"
#include "Chunk.h"
#include "System/Comparators/ivec2Comparator.h"
#include <thread>
#include <mutex>
#include <queue>
using namespace TealEngine;

class Element;

class World : public Component 
{
	private:
	std::map<glm::ivec2, Chunk*, ivec2Comparator> chunks;
	int chunkWidth, chunkHeight;
	glm::ivec2 cachedChunkIndex;
	Chunk* cachedChunk;
	public:
	static float gravity;
	GameNode3D* chunkLoader;

	std::queue<glm::ivec2> chunksToGenerate;
	std::mutex chunksToGenerateQueueLock;
	bool terminateGeneration;

	std::thread chunkGenThread;

	void chunkGenLoop();

	static WorldElement boundsElement;

	World();
	~World();

	void update() override;

	void setPixelColor(glm::vec2 at, glm::vec4 color);
	glm::ivec2 getChunkIndex(glm::ivec2 cellIndex) const;
	glm::ivec2 getCellIndex(glm::vec2 at) const;


	WorldElement& getElement(glm::vec2 position);

	void putElement(glm::vec2 at, Element* element, bool update = false, bool updatePixel = true);
	void eraseElement(glm::vec2 at, bool updatePixel = true);

	void makeParticle(glm::vec2 at);
	void stopParticle(glm::vec2 at);

	void loadChunk(glm::ivec2 index);

	void generateChunk(Chunk* chunk, glm::ivec2 index);

	void onAttach() override;
};