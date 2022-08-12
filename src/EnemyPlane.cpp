#include "EnemyPlane.h"
#include "PhongShader.h"
#include "NetworkConnector.h"

EnemyPlane::EnemyPlane(const char* srv_Adr,int port) 
{
	std::cout << "[Enemy] Enemy Plane Spawned..." << std::endl;

	Enemy_Position = Vector(-1, 3, -2.0f);
	
	Matrix m, r, s;

	m.translation(Enemy_Position);
	r.rotationY(3.1415f);
	s.scale(.8f);

	this->transform(m * r * s);
	NetworkConnector* nwc = new NetworkConnector(*this,srv_Adr,port);
	
}

void EnemyPlane::draw(const BaseCamera& cam)
{
	for (int i = 0; i < ENEMY_MODEL_AMOUNT; i++) {
		this->models[i]->draw(cam);
	}

}

void EnemyPlane::update(double delta) 
{
	Matrix OnlineMovement;

	/* Get Values here */
	OnlineMovement.translation(this->Enemy_Position);
	/* Apply active Values here */
	this->model->transform(OnlineMovement);

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


