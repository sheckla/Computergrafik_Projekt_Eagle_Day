#include "ParticleInstance.h"
#include "Matrix.h"

ParticleInstance::ParticleInstance(double ttl_, ParticleSprite* model_, BaseShader* shader_,Vector origin) : TTL(ttl_), ParticleModel(model_)
{
	ParticleModel->shader(shader_);
	Matrix M_origin;
	M_origin.translation(origin);

	ParticleModel->transform(M_origin);
	ParticleModel->setStartScale(((double)rand() / (RAND_MAX)) + 1);
}

void ParticleInstance::update(double deltaTime)
{
	TimeAlive += deltaTime;

	float scaling = ((1 - (TimeAlive / TTL)) * ParticleModel->getStartScale()); //Makes particle smaller, the older it gets
	//if (scaling < .3f)scaling = .3f;

	ParticleModel->setScale(scaling);
}

void ParticleInstance::draw(const BaseCamera& Cam)
{
	ParticleModel->draw(Cam);
}

bool ParticleInstance::isAlive() 
{
	return TimeAlive < TTL;
}