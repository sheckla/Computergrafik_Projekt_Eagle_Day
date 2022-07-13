#include "PlaneLoader.h"

Plane::Plane()
{

}

Plane::~Plane()
{
}

void Plane::loadModels(const char* path)
{
	this->model = new Model(path, false);
	PhongShader* pPhongShader = new PhongShader();
	model->shader(pPhongShader, true);
}

void Plane::draw(const BaseCamera& cam)
{
	this->model->draw(cam);
}
