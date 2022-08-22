#ifndef ParticleInstance_h
#define ParticleInstance_h

#include "Camera.h"
#include "BaseShader.h"
#include "vector.h"
#include "ParticleSprite.h"


class ParticleInstance
{
  
public:
    ParticleInstance::ParticleInstance(double ttl_, ParticleSprite* model_, BaseShader* shader_, Vector origin);

    ParticleInstance::~ParticleInstance() { delete ParticleModel; }

    virtual void ParticleInstance::update(double deltaTime);
    void ParticleInstance::draw(const BaseCamera& Cam);
    bool isAlive();
protected:
    double TimeAlive = 0;
    double TTL;
    ParticleSprite* ParticleModel;
};

#endif