#ifndef ParticleInstanceSmoke_h
#define ParticleInstanceSmoke_h

#include "BaseShader.h"
#include "ParticleSprite.h"
#include "ParticleInstance.h"


class ParticleInstanceSmoke : ParticleInstance
{
  
public:
    ParticleInstanceSmoke::ParticleInstanceSmoke(double ttl_, ParticleSprite* model_, BaseShader* shader_, Vector origin);

    ParticleInstanceSmoke::~ParticleInstanceSmoke() { ParticleInstance::~ParticleInstance(); }

    void ParticleInstanceSmoke::update(double deltaTime);
};

#endif