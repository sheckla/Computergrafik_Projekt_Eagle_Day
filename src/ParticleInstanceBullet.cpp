#include "ParticleInstanceBullet.h"
#include "Matrix.h"
#include "vector.h"
#include "CollisionDetector.h"

ParticleInstanceBullet::ParticleInstanceBullet(double ttl_, ParticleSprite* model_, BaseShader* shader_,Vector offset, Matrix transf, bool hitDetection) : ParticleInstance(ttl_,model_,shader_,offset)
{
	this->CurrentTransform = transf;
	this->hasHitDetection = hitDetection;

	Matrix offsetLeftRight,random_yaw,random_pitch,random_roll,offsetFrontBack;
	offsetLeftRight.translation(offset);
	offsetFrontBack.translation(transf.backward() * 2.0f);

	float max_abweichung = .014f;

	random_yaw.rotationY(  max_abweichung - ((double)rand() / (RAND_MAX)) * max_abweichung*2);
	random_pitch.rotationX(max_abweichung - ((double)rand() / (RAND_MAX)) * max_abweichung*2);
	random_roll.rotationZ( max_abweichung - ((double)rand() / (RAND_MAX)) * max_abweichung*2);


	this->CurrentTransform = offsetLeftRight * offsetFrontBack * this->CurrentTransform * random_yaw * random_pitch * random_roll;
}

void ParticleInstanceBullet::update(double deltaTime)
{
	ParticleInstance::update(deltaTime); //Important, do not change!

	Matrix forward_movement,gravitational_pull;
	forward_movement.translation(this->CurrentTransform.forward() * deltaTime * 400); //Forward movement
															//    9.81f m/s^2
	gravitational_pull.translation(Vector(0, -1, 0) * TimeAlive * 9.81f * deltaTime); //Gravitational downwards acceleration

	this->CurrentTransform = forward_movement * gravitational_pull * this->CurrentTransform;

	ParticleModel->transform(this->CurrentTransform);

	if (this->hasHitDetection) {
		if (this->hasHitSomething == false) // Can only hit once!
			if (CollisionDetector::BulletCollision(Vector(this->CurrentTransform.m03, this->CurrentTransform.m13, this->CurrentTransform.m23))) {
				this->hasHitSomething = true;
			}
	}
}
