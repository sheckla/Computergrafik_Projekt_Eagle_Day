#ifndef Plane_h
#define Plane_h

#include "Globals.h"
#include "basemodel.h"
#include "Model.h"
#include "Camera.h"
#include "PhongShader.h"
#include <stdlib.h>

const int PLANE_MODEL_AMOUNT = 7;
const float EPSILON = 1e-1;
const int DELTA_TIME_MULTIPLICATOR = 3;




class Plane : public BaseModel
{
    Vector modelPos[7];
	Model* models[PLANE_MODEL_AMOUNT];
	Model* model; // 1
	Model* rotor; // 2
	Model* rudder; // 3
	Model* backwing_left; // 4
	Model* backwing_right; // 5
	Model* wingflaps_left; // 6
	Model* wingflaps_right; // 7

	float speed = 0;
	float rudder_tilt = 0; 
	float left_flaps = 0;
	float right_flaps = 0;

	void stabilizeToZero(float& i, float max_angle);
public:
	float yaw = 0;
	float pitch = 0;
	float roll = 0;
	Plane();
	virtual ~Plane();
	void loadModels(const char* path);
	virtual void draw(const BaseCamera& cam);
	void update(double delta);
	void accelerate(float i);
	void leftFlapTilt(float i);
	void rightFlapTilt(float i);
	void rudderTilt(float i);
};

#endif

