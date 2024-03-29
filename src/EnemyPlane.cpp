#include "EnemyPlane.h"
#include "PhongShader.h"
#include "NetworkConnector.h"

#include "Aabb.h"
#include "MathUtil.h"
#include "ModelLoader.h"

EnemyPlane::EnemyPlane(const char* srv_Adr,int port) 
{
	std::cout << "[Enemy] Enemy Plane Spawned..." << std::endl;

	Vector Enemy_Position = Vector(0, 40, 0);
	
	Matrix m, r, s;

	m.translation(Enemy_Position);
	r.rotationY(3.1415f);
	s.scale(.3f);

	this->transform(m * r * s);

	Enemy_Tranformation.identity();
	if (ApplicationSettings::ONLINE_MODE) NetworkConnector* nwc = new NetworkConnector(*this,srv_Adr,port);
	if (!ApplicationSettings::ONLINE_MODE) initModelTranslation();

	//Instanciating all particle systems
	Gun_Left = new ParticleLoader(.01, 2, ParticleType::BulletDummy); //dummy bullets are not connected to the collision detector and are just visual
	Gun_Left->setOffset(-2.5f);

	Gun_Right = new ParticleLoader(.01, 2, ParticleType::BulletDummy); //dummy bullets are not connected to the collision detector and are just visual
	Gun_Right->setOffset(2.5f);

	Smoke_System = new ParticleLoader(.0002, .14, ParticleType::Smoke);

	Muzzleflash_Right = new ParticleLoader(.01, .03, ParticleType::MuzzleFlash);
	Muzzleflash_Right->setOffset(2.5f);

	Muzzleflash_Left = new ParticleLoader(.01, .03, ParticleType::MuzzleFlash);
	Muzzleflash_Left->setOffset(-2.5f);
}

const AABB& EnemyPlane::aabb() const
{
	//Creates 8x8 Hitbox that moves with the plane-body-position
	float posX = this->model->transform().m03;
	float posY = this->model->transform().m13;
	float posZ = this->model->transform().m23;
	Vector pos = model->transform().translation();
	AABB aabb;
	aabb.Min = Vector(-2 + pos.X, -2 + pos.Y, -2 + pos.Z);
	aabb.Max = Vector(2 + pos.X, 2 + pos.Y, 2 + pos.Z);
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
	//simply rotates rotor at constant speed, as it is difficult to see anyway
	rotorRotation.rotationZ(PI * delta * 100 * 5);
	previousRotorRotation = rotorRotation * previousRotorRotation;

	rotor_offset.translation(Vector(0, 0.245185f, 1.82053f - .15f));

	if (ApplicationSettings::ONLINE_MODE)
	{
		if(Enemy_Tranformation_Validation==true) //Last update is current
		{
			Matrix forward;
			forward.translation(Vector(0, 0, Enemy_Speed * 0.002f));

		this->model->transform(Enemy_Tranformation * forward);
		this->propeller->transform(Enemy_Tranformation * forward * rotor_offset * rotorRotation * previousRotorRotation);
		this->transform(model->transform());
	}
	else
	{
		//Motion Estimate! If the last update is older than one frame, the future position is estimated from the last plane transform an speed. 
		//Inaccuracies will be overriden by next (server)update
		Matrix forward;
		forward.translation(Vector(0, 0, Enemy_Speed * 0.002f));

			this->model->transform(this->model->transform() * forward);
			this->propeller->transform(this->model->transform() * forward * rotor_offset * rotorRotation * previousRotorRotation);
			this->transform(model->transform());
			//std::cout << "[Enemy] Last update one frame behind: <Motion Estimate>" << std::endl;
		}
	} else if (ModelLoader::pAIPlane && !ApplicationSettings::ONLINE_MODE)
	{
		Matrix forward;
		forward.translation(Vector(0, 0, Enemy_Speed * 0.002f));

		this->model->transform(transform() * forward);
		this->propeller->transform(transform() * forward * rotor_offset * rotorRotation * previousRotorRotation);
		this->transform(model->transform());
	}
	
	Enemy_Tranformation_Validation = false;

	if(isShooting)
	{
		this->Gun_Left->StartGenerating();
		this->Gun_Right->StartGenerating();

		this->Muzzleflash_Left->StartGenerating();
		this->Muzzleflash_Right->StartGenerating();
	}
	else 
	{
		this->Gun_Left->StopGenerating();
		this->Gun_Right->StopGenerating();

		this->Muzzleflash_Left->StopGenerating();
		this->Muzzleflash_Right->StopGenerating();
	}

	this->Gun_Left->update(delta, this->model->transform());
	this->Gun_Right->update(delta, this->model->transform());

	(this->hp < 50.0f) ? Smoke_System->StartGenerating() : Smoke_System->StopGenerating();
	Smoke_System->update(delta, this->propeller->transform());

	Muzzleflash_Right->update(delta, this->model->transform());
	Muzzleflash_Left->update(delta, this->model->transform());
}

void EnemyPlane::loadModels(const char* path)
{
	std::string planePath = path;
	bool fitToSize = false;

	//As details cant really be seen, the enemy plane is much simpler than the player plane.
	//The enemy plane is only made out of its body and its rotor
	this->model = new Model(&(planePath + "/messerschmitt.obj")[0], fitToSize);
	PhongShader* shader = new PhongShader();
	shader->phongDiff(2.1);
	shader->ambientColor(Color(0, 0, 0));
	this->model->shader(shader, true);

	models[0] = this->model;
	models[0]->transform(models[0]->transform() * Matrix().scale(0.3, 0.3, 0.3));

	
	this->propeller = new Model(&(planePath + "/propeller.obj")[0], fitToSize);
	this->propeller->shader(shader, true);
	models[1] = this->propeller;
	models[1]->transform(models[1]->transform() * Matrix().scale(0.3, 0.3, 0.3));
	
}

void EnemyPlane::initModelTranslation()
{
	// Offsets anwenden
	if (!ModelLoader::pPlayerPlane) return;
	Vector offset = Vector(ModelLoader::pPlayerPlane->getParts()[0]->transform().translation()) + Vector(0,0, 10);
	this->transform(Matrix().translation(offset) * Matrix().scale(0.3, 0.3, 0.3));
	hp = 100;
}


