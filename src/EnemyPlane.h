#include "basemodel.h"
#include "Model.h"
#pragma once

const int ENEMY_MODEL_AMOUNT = 1;

class EnemyPlane : public BaseModel
{
	Model* models[ENEMY_MODEL_AMOUNT];
	Model* model;

public:
	EnemyPlane();
	virtual void draw(const BaseCamera& cam);
	void update(double delta);
	void EnemyPlane::loadModels(const char* path);
};

