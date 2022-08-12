#ifndef EnemyPlane_h
#define EnemyPlane_h

#include "basemodel.h"
#include "Model.h"
#include "NetworkConnector.h"

const int ENEMY_MODEL_AMOUNT = 1;

class EnemyPlane : public BaseModel
{
	Model* models[ENEMY_MODEL_AMOUNT];
	Model* model;
	//NetworkConnector* nwc; Gab mir mir Fehler aufgrund #include. Code läuft aber
	float s;

public:
	Vector Enemy_Position;
	EnemyPlane::EnemyPlane(const char* srv_Adr,int port);
	virtual void draw(const BaseCamera& cam);
	void update(double delta);
	void EnemyPlane::loadModels(const char* path);

	
};

#endif