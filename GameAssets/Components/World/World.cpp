#include "World.h"
#include "Particle.h"
#include "Element.h"
#include "GameNode/GameNode3D.h"
#include "Elements/Sand.h"
#include "Math/PerlinNoise.h"
#include "Math/randrange.h"
#include <thread>
#include <chrono>
#include "Elements/Stone.h"
#include "Elements/Soil.h"
#include "Elements/Grass.h"
#include "Elements/Water.h"

#define GridBias (0.0001f)

WorldElement World::boundsElement = WorldElement();
float World::gravity = 0.03f;

World::World() 
{
	Particle::stopAll();
	chunkWidth = 320;
	chunkHeight = 240;
	Particle::world = this;
	chunkLoader = nullptr;
	terminateGeneration = false;
	chunkGenThread = std::thread(&World::chunkGenLoop, this);
}

void World::chunkGenLoop() 
{
	while(!terminateGeneration) 
	{
		glm::ivec2 index;
		Chunk* chunk = nullptr;
		if(chunksToGenerate.size()) 
		{
			std::lock_guard<std::mutex> guard(this->chunksToGenerateQueueLock);
			index = chunksToGenerate.front();
			if(chunks.find(index) != chunks.cend()) 
			{
				chunk = chunks[index];
			}
			chunksToGenerate.pop();
		}

		if(chunk) 
		{
			this->generateChunk(chunk, index);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
	}
}

glm::ivec2 World::getCellIndex(glm::vec2 at) const 
{
	return glm::ivec2(glm::floor(at) + sign(at) * GridBias);
}

glm::ivec2 World::getChunkIndex(glm::ivec2 cellIndex) const
{
	glm::ivec2 chunkIndex = cellIndex / glm::ivec2(chunkWidth, chunkHeight);

	if(cellIndex.x < 0) 
	{
		chunkIndex.x = ((cellIndex.x + 1) / chunkWidth) - 1;
	}
	if(cellIndex.y < 0) 
	{
		chunkIndex.y = ((cellIndex.y + 1) / chunkHeight) - 1;
	}

	return chunkIndex;
}

void World::setPixelColor(glm::vec2 at, glm::vec4 color) 
{
	glm::ivec2 elementGlobalIndex = getCellIndex(at);
	glm::ivec2 chunkIndex = getChunkIndex(elementGlobalIndex);
	
	auto chunkIt = chunks.find(chunkIndex);
	if(chunkIt == chunks.cend()) return;
	chunkIt->second->setPixelColor(
		elementGlobalIndex - chunkIndex * glm::ivec2(chunkWidth, chunkHeight), 
		glm::ivec4(color * 254.0f)
		);
}

WorldElement& World::getElement(glm::vec2 position) 
{
	glm::ivec2 elementGlobalIndex = getCellIndex(position);
	glm::ivec2 chunkIndex = getChunkIndex(elementGlobalIndex);

	Chunk* chunk = nullptr;

	if((chunkIndex == cachedChunkIndex) && chunk)
	{
		chunk = cachedChunk;
	}
	else 
	{

		auto chunkIt = chunks.find(chunkIndex);

		if(chunkIt == chunks.cend()) return World::boundsElement;

		chunk = chunkIt->second;
	}
	
	glm::ivec2 elementIndex = elementGlobalIndex - chunkIndex * glm::ivec2(chunkWidth, chunkHeight);
	
	cachedChunk = chunk;
	cachedChunkIndex = chunkIndex;
 	return chunk->getElement(elementIndex);
}

void World::putElement(glm::vec2 at, Element* element, bool update, bool updatePixel) 
{
	WorldElement& elRef = getElement(at);
	if(&elRef == &World::boundsElement || elRef.element) 
	{
		delete element;
		return;
	}
	if(updatePixel) setPixelColor(at, element->color);
	elRef.element = element;
	element->worldElement = &elRef;
	if(update) 
	{
		element->updateBehaviour(at, Particle::getById(0));
		glm::vec2 relativeUpdate[] = {glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(0.0f, 1.0f)};
		for(int i = 0; i < 4; i++) 
		{
			glm::vec2 index = at + relativeUpdate[i];

			Element* el = getElement(index).element;
			if(el) el->updateBehaviour(index, Particle::getById(0));
		}
	}
}

void World::eraseElement(glm::vec2 at, bool updatePixel) 
{
	WorldElement& elRef = getElement(at);
	delete elRef.element;
	elRef.element = nullptr;
	if(elRef.particleId) 
	{
		Particle::getById(elRef.particleId).stopParticle();
	}
	elRef.particleId = 0;

	if(updatePixel) setPixelColor(at, glm::vec4(0.0f));
 	
	WorldElement& r = getElement(at + glm::vec2(1.0f, 0.0f));
	WorldElement& l = getElement(at - glm::vec2(1.0f, 0.0f));
	WorldElement& u = getElement(at + glm::vec2(0.0f, 1.0f));
	WorldElement& d = getElement(at - glm::vec2(0.0f, 1.0f));

	if(r.element) r.element->updateBehaviour(at + glm::vec2(1.0f, 0.0f), Particle::getById(0));
	if(l.element) l.element->updateBehaviour(at - glm::vec2(1.0f, 0.0f), Particle::getById(0));
	if(u.element) u.element->updateBehaviour(at + glm::vec2(0.0f, 1.0f), Particle::getById(0));
	if(d.element) d.element->updateBehaviour(at - glm::vec2(0.0f, 1.0f), Particle::getById(0));
}

void World::makeParticle(glm::vec2 at) 
{
	WorldElement& elRef = getElement(at);
	elRef.particleId = Particle::create(glm::floor(at) + 0.5f, elRef.element).getId();
}
void World::stopParticle(glm::vec2 at) 
{
	WorldElement& elRef = getElement(at);
	Particle::getById(elRef.particleId).stopParticle();
	elRef.particleId = 0;
}

void World::loadChunk(glm::ivec2 index) 
{
	GameNode3D* chunkNode = new GameNode3D();
	chunkNode->getRelativeTransform().setPosition(glm::vec3(index.x * 2.0f, index.y * 2.0f, 0.0f));
	Chunk* chunk = new Chunk(this->chunkWidth, this->chunkHeight);
	
	chunkNode->attachComponent(chunk);
	this->getParent()->addChild(chunkNode);
	this->chunks[index] = chunk;

	std::lock_guard<std::mutex> guard(this->chunksToGenerateQueueLock);
	chunksToGenerate.push(index);
}

void World::generateChunk(Chunk* chunk, glm::ivec2 index) 
{
	const float biomesSize = 2000.0f;
	for(int i = 0; i < chunkWidth; i++) 
	{
		float humidity = PerlinNoise::get(((float)( i + index.x * chunkWidth )) / biomesSize, 8.0f, 12.0f);
		float height = PerlinNoise::get(((float)( i + index.x * chunkWidth )) / 300.0f, 0.0f, 3.0f) * 120.0f;
		height += PerlinNoise::get(((float)( i + index.x * chunkWidth )) / 1000.0f, 0.0f, 3.0f) * 600.0f + 200.0f;

		for(int j = 0; j < chunkHeight; j++) 
		{
			float depth = height - (j + index.y * chunkHeight);
			if(depth > 0.0f) 
			{
				Element* element = nullptr;
				if(depth > 50.0f + randrange(-2.0f, 2.0f) + PerlinNoise::get(((float)( i + index.x * chunkWidth)) / 25.0f, (float)(j + index.y * chunkHeight) / 25.0f)*30.0f)
				{
					if(index.y < -1) 
					{
						if(PerlinNoise::get(((float)( i + index.x * chunkWidth)) / 128.0f, ((float)(j + index.y * chunkHeight)) / 128.0f) > 1.0f / (1.0f + ((float)(j + index.y * chunkHeight))/100.0f)) 
						{
							continue;
						}
					}
					element = new Stone();
				}
				else 
				{
					humidity += PerlinNoise::get(((float)( i + index.x * chunkWidth)) / 6.0f, ((float)(j + index.y * chunkHeight)) / 6.0f) * 0.01f;
					if(humidity > 0.0f) 
					{
						element = new Sand();
						//element->color = glm::vec4(humidity);
					}
					else 
					{
						if(depth < 2) 
						{
							element = new Grass();
						}
						else 
						{
							element = new Soil();
						}
						//element->color = glm::vec4(humidity);
					}

				}
				if(element)
				this->putElement(glm::vec2(i + chunkWidth * index.x, j + chunkHeight * index.y) + 0.5f, element, false, false);
			}
			else if((j + index.y * chunkHeight) < 0)
			{
				Element* element = new Water();
				this->putElement(glm::vec2(i + chunkWidth * index.x, j + chunkHeight * index.y) + 0.5f, element, false, false);
			}
		}
	}

	chunk->updateDisplayRequired = true;
}

void World::update() 
{
	const int loadDistance = 4;
	if(this->chunkLoader) 
	{
		glm::vec3 position3 = chunkLoader->getWorldTransform().getPosition();
		glm::vec2 position = glm::vec2(position3.x, position3.y);
		
		glm::ivec2 chunkLoaderChunk = glm::ivec2(
			position.x / chunkWidth,
			position.y / chunkHeight
		);

		if(position.x < 0.0f) chunkLoaderChunk.x -= 1;
		if(position.y < 0.0f) chunkLoaderChunk.y -= 1;
		std::vector<glm::ivec2> chunksToUnload;
		for(auto chunkPair : chunks) 
		{
			if(
				chunkPair.first.x < chunkLoaderChunk.x-loadDistance-1 ||
				chunkPair.first.x > chunkLoaderChunk.x+loadDistance+1 ||
				chunkPair.first.y < chunkLoaderChunk.y-loadDistance-1 ||
				chunkPair.first.y > chunkLoaderChunk.y+loadDistance+1
			) 
			{
				chunksToUnload.push_back(chunkPair.first);
			}
		}
		for(glm::ivec2 index : chunksToUnload) 
		{
			this->chunks[index]->getParent()->destroy();
			this->chunks.erase(index);
		}

		if(chunks.find(glm::ivec2(chunkLoaderChunk.x, chunkLoaderChunk.y)) == chunks.cend()) 
		{
			loadChunk(glm::ivec2(chunkLoaderChunk.x, chunkLoaderChunk.y));
		}

		for(int i = 0; i <= loadDistance; i++) 
		{
			for(int j = 0; j <= loadDistance; j++) 
			{
				if(i || j) 
				{
					if(chunks.find(glm::ivec2(chunkLoaderChunk.x + i, chunkLoaderChunk.y + j)) == chunks.cend()) 
					{
						loadChunk(glm::ivec2(chunkLoaderChunk.x + i, chunkLoaderChunk.y + j));
					}
					if(chunks.find(glm::ivec2(chunkLoaderChunk.x + i, chunkLoaderChunk.y - j - 1)) == chunks.cend()) 
					{
						loadChunk(glm::ivec2(chunkLoaderChunk.x + i, chunkLoaderChunk.y - j - 1));
					}
					if(chunks.find(glm::ivec2(chunkLoaderChunk.x - i - 1, chunkLoaderChunk.y + j)) == chunks.cend()) 
					{
						loadChunk(glm::ivec2(chunkLoaderChunk.x - i - 1, chunkLoaderChunk.y + j));
					}
					if(chunks.find(glm::ivec2(chunkLoaderChunk.x - i - 1, chunkLoaderChunk.y - j - 1)) == chunks.cend()) 
					{
						loadChunk(glm::ivec2(chunkLoaderChunk.x - i - 1, chunkLoaderChunk.y - j - 1));
					}
				}
			}	
		}
	}
	for(const auto& chunkPair : chunks) 
	{
		if(chunkPair.second->updateDisplayRequired) 
		{
			chunkPair.second->updateDisplay();
			chunkPair.second->updateDisplayRequired = false;
		}
	}
	Particle::updateAll();
}

void World::onAttach() 
{
}

World::~World() 
{
	terminateGeneration = true;
	this->chunkGenThread.join();
}

EXPORT_COMPONENT(World);