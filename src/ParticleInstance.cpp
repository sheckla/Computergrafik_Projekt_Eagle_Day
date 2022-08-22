#include "ParticleInstance.h"
#include "Matrix.h"

ParticleInstance::ParticleInstance(double ttl_, ParticleSprite* model_, BaseShader* shader_,Vector origin) : TTL(ttl_), ParticleModel(model_)
{
	ParticleModel->shader(shader_);

	Matrix M_origin;
	M_origin.translation(origin);

	ParticleModel->transform(M_origin);
}

void ParticleInstance::update(double deltaTime)
{
	TimeAlive += deltaTime;
}

void ParticleInstance::draw(const BaseCamera& Cam)
{
	ParticleModel->draw(Cam);
}

bool ParticleInstance::isAlive() 
{
	return TimeAlive < TTL;
}