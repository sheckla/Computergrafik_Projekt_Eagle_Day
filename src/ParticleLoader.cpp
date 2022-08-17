#include "ModelLoader.h"

#include "ParticleLoader.h"
#include "ParticleSprite.h"
#include "ConstantShader.h"

ParticleLoader::ParticleLoader(float particlesEveryXSeconds_,double ttl) : ParticlesEveryXSeconds(particlesEveryXSeconds_),Particle_TTL(ttl)
{
	PhongShader* ps = new PhongShader(true);
	ps->diffuseTexture(new Texture("G:/Repository/Eagle_Day/assets/smokeparticle.png"));

	ParticleShader = ps;
	
}

void ParticleLoader::update(double deltaTime, Vector origin) 
{
	Threshhold += deltaTime;
	if (Threshhold > ParticlesEveryXSeconds)
	{							
		ParticleInstance* instance = new ParticleInstance(Particle_TTL, new ParticleSprite(), ParticleShader,origin);
		ParticleList.push_back(instance);
		Threshhold = 0;
	}

	
	for (auto const& it : ParticleList)
		it->update(deltaTime);

	std::list<ParticleInstance*>::iterator it = ParticleList.begin();
	std::list<ParticleInstance*>::iterator end = ParticleList.end();
	while (it != end)
	{
		bool alive = ((*it)->isAlive());

		std::list<ParticleInstance*>::iterator previous = it;

		it++;

		if (!alive) {
			ParticleInstance* instance = ParticleList.front();
			//ParticleList.pop_front();
			ParticleList.erase(previous);
			delete instance;
		}
	}
}

void ParticleLoader::draw(const BaseCamera& Cam)
{
	std::list<ParticleInstance*>::iterator it = ParticleList.begin();
	std::list<ParticleInstance*>::iterator end = ParticleList.end();
	while (it != end)
	{
		(*it)->draw(Cam);
		it++;
	}
}