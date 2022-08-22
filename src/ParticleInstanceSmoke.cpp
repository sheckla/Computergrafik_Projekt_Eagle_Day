#include "ParticleInstanceSmoke.h"
#include "Matrix.h"

ParticleInstanceSmoke::ParticleInstanceSmoke(double ttl_, ParticleSprite* model_, BaseShader* shader_,Vector origin) : ParticleInstance(ttl_,model_,shader_,origin)
{
	ParticleModel->setStartScale(((double)rand() / (RAND_MAX)) + 1);
}

void ParticleInstanceSmoke::update(double deltaTime)
{
	ParticleInstance::update(deltaTime); //Important, do not change!

	float scaling = ((1 - (TimeAlive / TTL)) * ParticleModel->getStartScale()); //Makes particle smaller, the older it gets
	ParticleModel->setScale(scaling);
}
