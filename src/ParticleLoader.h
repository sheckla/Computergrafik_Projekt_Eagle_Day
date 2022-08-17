#ifndef ParticleLoader_h
#define ParticleLoader_h

#include "Camera.h"
#include "ParticleInstance.h"
#include "PhongShader.h"
#include "vector.h"

class ParticleLoader
{
public:
    ParticleLoader::ParticleLoader(float particlesPerSecond_, double ttl);
    void ParticleLoader::update(double deltaTime,Vector origin);
    void ParticleLoader::draw(const BaseCamera& Cam);
private:
    float ParticlesEveryXSeconds;
    double Threshhold = 0;
    BaseShader* ParticleShader;
    double Particle_TTL;

    std::list<ParticleInstance*> ParticleList;
};

#endif