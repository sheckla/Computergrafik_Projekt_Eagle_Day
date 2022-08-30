/*
 * Gegnerisches Flugzeug
 * - im Mehrspielermodus = Spieler 2
 */

#ifndef EnemyPlane_h
#define EnemyPlane_h

#include "basemodel.h"
#include "Model.h"
#include "Matrix.h"
#include "ParticleLoader.h"

const int ENEMY_MODEL_AMOUNT = 2;

class EnemyPlane : public BaseModel
{
	Model* models[ENEMY_MODEL_AMOUNT];
	Model* model;
	Model* propeller;

public:

	Matrix Enemy_Tranformation;
	bool Enemy_Tranformation_Validation = false;
	float Enemy_Speed=0;

	const AABB& EnemyPlane::boundingBox() const;

	EnemyPlane();
	EnemyPlane::EnemyPlane(const char* srv_Adr,int port);
	virtual void draw(const BaseCamera& cam);
	void update(double delta);
	void EnemyPlane::loadModels(const char* path);

	void EnemyPlane::hit() { 
		this->hp = this->hp - 2.2f;
	}

	bool isShooting = false;

	void drawParticles(const BaseCamera& Cam) {
		this->Gun_Left->draw(Cam);
		this->Gun_Right->draw(Cam);
		this->Smoke_System->draw(Cam);

		this->Muzzleflash_Right->draw(Cam);
		this->Muzzleflash_Left->draw(Cam);
	}
	float hp=100.0f;

private:
	
	Matrix previousRotorRotation = Matrix().rotationZ(0);
	ParticleLoader* Gun_Left;
	ParticleLoader* Gun_Right;

	ParticleLoader* Muzzleflash_Right;
	ParticleLoader* Muzzleflash_Left;

	ParticleLoader* Smoke_System;

};

#endif