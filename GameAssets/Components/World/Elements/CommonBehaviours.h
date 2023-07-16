#pragma once
#include "Components/World/Particle.h"

void sandParticleBehaviour(Element* element, Particle& particle, float spreadVelocity);
void fluidParticleBehaviour(Particle& particle);

void fallingActivityUpdateBehaviour(Element* element, glm::vec2 at, const Particle& by, int updatesPerActivationChance = 1);