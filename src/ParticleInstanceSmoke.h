/*
 * ParticleInstanceSmoke
 * - Child of ParticleInstance
 * - Shrinks particle over the course of its live
 */

#ifndef ParticleInstanceSmoke_h
#define ParticleInstanceSmoke_h

#include "BaseShader.h"
#include "ParticleSprite.h"
#include "ParticleInstance.h"


class ParticleInstanceSmoke : public ParticleInstance
{
  
public:
    ParticleInstanceSmoke::ParticleInstanceSmoke(double ttl_, ParticleSprite* model_, BaseShader* shader_, Vector origin);

    ParticleInstanceSmoke::~ParticleInstanceSmoke() {  }

    void ParticleInstanceSmoke::update(double deltaTime);
};

#endif