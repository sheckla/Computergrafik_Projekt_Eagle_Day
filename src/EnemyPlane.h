#ifndef EnemyPlane_h
#define EnemyPlane_h

#include "basemodel.h"
#include "Model.h"
#include "Matrix.h"
//#include "NetworkConnector.h"

const int ENEMY_MODEL_AMOUNT = 1;

class EnemyPlane : public BaseModel
{
	Model* models[ENEMY_MODEL_AMOUNT];
	Model* model;
	//NetworkConnector* nwc; //Gab mir mir Fehler aufgrund #include. Code läuft aber
	//float s;

public:
	/*
	Vector Enemy_Position;
	Vector Enemy_Rotation_1;
	Vector Enemy_Rotation_2;
	Vector Enemy_Rotation_3;
	Vector Enemy_Rotation_4;
	*/
	Matrix Enemy_Tranformation;
	bool Enemy_Tranformation_Validation = false;
	float Enemy_Speed=0;

	EnemyPlane::EnemyPlane(const char* srv_Adr,int port);
	virtual void draw(const BaseCamera& cam);
	void update(double delta);
	void EnemyPlane::loadModels(const char* path);
};

#endif