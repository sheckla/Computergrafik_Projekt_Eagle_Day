#ifndef ParticleInstanceBullet_h
#define ParticleInstanceBullet_h

#include "BaseShader.h"
#include "ParticleSprite.h"
#include "ParticleInstance.h"
#include "Matrix.h"


class ParticleInstanceBullet : ParticleInstance
{
  
public:
    ParticleInstanceBullet::ParticleInstanceBullet(double ttl_, ParticleSprite* model_, BaseShader* shader_, Vector origin,Matrix transf, bool hitDetection);

    ParticleInstanceBullet::~ParticleInstanceBullet() { ParticleInstance::~ParticleInstance(); }

    void ParticleInstanceBullet::update(double deltaTime);
private:
    Matrix CurrentTransform;
    bool hasHitSomething = false;
    bool hasHitDetection = false;
};

#endif