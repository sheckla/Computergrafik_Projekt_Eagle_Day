#ifndef Plane_h
#define Plane_h

#include "Globals.h"
#include "basemodel.h"
#include "Model.h"
#include "Camera.h"
#include "PhongShader.h"

class Plane : public BaseModel
{
	Model* model;
public:
	Plane();
	virtual ~Plane();
	void loadModels(const char* path);
	virtual void draw(const BaseCamera& cam);
};

#endif

