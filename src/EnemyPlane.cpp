#include "EnemyPlane.h"
#include "PhongShader.h"
#include "NetworkConnector.h"

#include "Aabb.h"

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

	Gun_Left = new ParticleLoader(.01, 2, ParticleType::BulletDummy);
	Gun_Left->setOffset(-2.5f);

	Gun_Right = new ParticleLoader(.01, 2, ParticleType::BulletDummy);
	Gun_Right->setOffset(2.5f);

	Smoke_System = new ParticleLoader(.02, .5, ParticleType::Smoke);
}

const AABB& EnemyPlane::boundingBox() const
{
	float posX = this->model->transform().m03;
	float posY = this->model->transform().m13;
	float posZ = this->model->transform().m23;
	
	AABB aabb;
	aabb.Min = Vector(-4 + posX, -4 + posY, -4 + posZ);
	aabb.Max = Vector(4 + posX, 4 + posY, 4 + posZ);
	return aabb;
}

void EnemyPlane::draw(const BaseCamera& cam)
{
	for (int i = 0; i < ENEMY_MODEL_AMOUNT; i++) {
		this->models[i]->draw(cam);
	}
}

void EnemyPlane::update(double delta)
{
	Matrix rotorRotation,rotor_offset;
	rotorRotation.rotationZ(PI * delta * 100 * 5);
	previousRotorRotation = rotorRotation * previousRotorRotation;

	rotor_offset.translation(Vector(0, 0.245185f, 1.82053f - .15f));


	if(Enemy_Tranformation_Validation==true) //Last update is current
	{
		Matrix forward;
		forward.translation(Vector(0, 0, Enemy_Speed * 0.002f));

		this->model->transform(Enemy_Tranformation * forward);
		this->propeller->transform(Enemy_Tranformation * forward * rotor_offset * rotorRotation * previousRotorRotation);
	}
	else
	{
		//Motion Estimate!
		Matrix forward;
		forward.translation(Vector(0, 0, Enemy_Speed * 0.002f));

		this->model->transform(this->model->transform() * forward);
		this->propeller->transform(this->model->transform() * forward * rotor_offset * rotorRotation * previousRotorRotation);
		//std::cout << "[Enemy] Last update one frame behind: <Motion Estimate>" << std::endl;
	}
	
	Enemy_Tranformation_Validation = false;

	if(isShooting)
	{
		this->Gun_Left->StartGenerating();
		this->Gun_Right->StartGenerating();
	}
	else 
	{
		this->Gun_Left->StopGenerating();
		this->Gun_Right->StopGenerating();
	}

	this->Gun_Left->update(delta, this->model->transform());
	this->Gun_Right->update(delta, this->model->transform());

	if (this->hp < 50.0f)Smoke_System->StartGenerating();
	else Smoke_System->StopGenerating();

	Smoke_System->update(delta, this->propeller->transform());
}

void EnemyPlane::loadModels(const char* path)
{
	std::string planePath = path;
	bool fitToSize = false;

	this->model = new Model(&(planePath + "/messerschmitt.obj")[0], fitToSize);
	PhongShader* shader = new PhongShader();
	this->model->shader(shader, true);

	models[0] = this->model;

	
	this->propeller = new Model(&(planePath + "/propeller.obj")[0], fitToSize);
	this->propeller->shader(shader, true);
	models[1] = this->propeller;
	
}


