#ifndef ParticleLoader_h
#define ParticleLoader_h

#include "Camera.h"
#include "ParticleInstance.h"
#include "PhongShader.h"
#include "vector.h"
#include <list>
#include "Matrix.h"

enum ParticleType { Smoke, Bullet,BulletDummy,MuzzleFlash }; //Ableitungstypen von ParticleInstance, für weitere Partikelarten hinzufügen...

class ParticleLoader
{
public:
    ParticleLoader::ParticleLoader(float particlesPerSecond_, double ttl,ParticleType type);
    ParticleLoader::~ParticleLoader() {/* TODO */ }
    void ParticleLoader::update(double deltaTime,Matrix origin);
    void ParticleLoader::draw(const BaseCamera& Cam);

    void ParticleLoader::StartGenerating() { this->IsGenerating = true; }
    void ParticleLoader::StopGenerating() { this->IsGenerating = false; }

    void setOffset(float offset) { this->Offset_Value = offset; }
    void setScale(float scale);
private:
    float ParticlesEveryXSeconds;
    double Threshold = 0;
    BaseShader* Particle_Shader;
    double Particle_TTL;

    float Offset_Value = 0;

    const ParticleType pType;

    std::list<ParticleInstance*> ParticleList;
    bool IsGenerating = false;

    float Scale = 1.0f;
};

#endif