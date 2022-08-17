#include "EnemyPlane.h"
#include "PhongShader.h"
#include "NetworkConnector.h"

EnemyPlane::EnemyPlane(const char* srv_Adr,int port) 
{
	std::cout << "[Enemy] Enemy Plane Spawned..." << std::endl;

	Vector Enemy_Position = Vector(-1, 3, -2.0f);
	
	Matrix m, r, s;

	m.translation(Enemy_Position);
	r.rotationY(3.1415f);
	s.scale(.8f);

	this->transform(m * r * s);

	Enemy_Tranformation.identity();
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
	if(Enemy_Tranformation_Validation==true) //Last update is current
	{
		this->model->transform(Enemy_Tranformation);
	}
	else
	{
		//Motion Estimate!
		Matrix forward;
		forward.translation(Vector(0, 0, Enemy_Speed * delta));
		this->model->transform(this->model->transform() * forward);
		//std::cout << "[Enemy] Last update one frame behind: <Motion Estimate>" << std::endl;
	}
	Enemy_Tranformation_Validation = false;
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


