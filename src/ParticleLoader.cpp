#include "ParticleLoader.h"
#include "ParticleSprite.h"
#include "ParticleShader.h"
#include "ParticleInstanceSmoke.h"
#include "ParticleInstanceBullet.h"
#include "ParticleInstance.h"
#include "Matrix.h"
#include "vector.h"

ParticleLoader::ParticleLoader(float particlesEveryXSeconds_,double ttl, ParticleType type) : ParticlesEveryXSeconds(particlesEveryXSeconds_),Particle_TTL(ttl),pType(type)
{
	ParticleShader* ps = new ParticleShader();

	const std::string Path = ASSETS "models/spitfire";

	if(pType == ParticleType::Smoke)
		ps->diffuseTexture(new Texture((Path +"/particles/smokeparticle.png").c_str())); // SMOKE-TEXTURE

	if (pType == ParticleType::Bullet || pType == ParticleType::BulletDummy)
		ps->diffuseTexture(new Texture((Path + "/particles/smokeparticle.png").c_str())); // GUN-TEXTURE

	Particle_Shader = ps;
}

void ParticleLoader::update(double deltaTime, Matrix origin) 
{
	if (IsGenerating == true) {
		Threshold += deltaTime;
		if (Threshold > ParticlesEveryXSeconds)
		{
			//For new ParticleTypes: new if-Block here! Update will be automatically called for new instance to define specific behaviour
			if (pType == ParticleType::Smoke) {
				ParticleInstanceSmoke* instance = new ParticleInstanceSmoke(Particle_TTL, new ParticleSprite(), Particle_Shader, Vector(origin.m03, origin.m13, origin.m23));
				ParticleList.push_back((ParticleInstance*)instance);
			}
			if (pType == ParticleType::Bullet) {
				ParticleInstanceBullet* instance = new ParticleInstanceBullet(Particle_TTL, new ParticleSprite(), Particle_Shader, origin.left() * this->Offset_Value, origin,true);
				ParticleList.push_back((ParticleInstance*)instance);
			}
			if (pType == ParticleType::BulletDummy) {
				ParticleInstanceBullet* instance = new ParticleInstanceBullet(Particle_TTL, new ParticleSprite(), Particle_Shader, origin.left() * this->Offset_Value, origin, false);
				ParticleList.push_back((ParticleInstance*)instance);
			}
			Threshold = 0;
		}
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