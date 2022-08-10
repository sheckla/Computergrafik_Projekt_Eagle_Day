#include "EnemyPlane.h"
#include "PhongShader.h"

EnemyPlane::EnemyPlane() 
{
	std::cout << "ENEMY" << std::endl;
	
	Matrix m, r, s;
	m.translation(Vector(-1, 3, -2.0f));
	r.rotationY(3.1415f);
	s.scale(.8f);

	//Matrix tR;
	//tR.rotationX(.6f);

	this->transform(m * r * s/* * tR*/);
	
}

void EnemyPlane::draw(const BaseCamera& cam)
{
	for (int i = 0; i < ENEMY_MODEL_AMOUNT; i++) {
		this->models[i]->draw(cam);
	}

}

void EnemyPlane::update(double delta) 
{
	/* Get Values here */
	Matrix m,r;
	m.translation(Vector(0, 0, -0.1f));
	r.rotationY(.0f);

	this->transform(transform() * r * m);
	
	this->model->transform(this->transform());
	//std::cout << "MOVE" << std::endl;
}

void EnemyPlane::loadModels(const char* path)
{
	std::string planePath = path;
	bool fitToSize = false;

	this->model = new Model(&(planePath + "/messerschmitt.obj")[0], fitToSize);
	PhongShader* shader = new PhongShader();
	this->model->shader(shader, true);

	models[0] = this->model;
}


