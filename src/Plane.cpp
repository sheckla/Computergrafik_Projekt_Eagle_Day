#include "PlaneLoader.h"

void Plane::stabilizeToZero(float& i, float max_angle)
{
	if (abs(i) <= EPSILON) {
		i = 0;
		return;
	};

	if (i >= max_angle) {
		i = max_angle - 1;
		return;
	}

	if (i <= -max_angle) {
		i = -max_angle + 1;
		return;
	}

	if (i >= 0) {
		i -= 0.1;
		return;
	}
	else if (i < 0) {
		i += 0.1;
		return;
	}
}

Plane::Plane()
{
	// Blender Y = -Z
	// Blender Z = Y
	modelPos[0] = Vector(0, 0, 0); // model
	modelPos[1] = Vector(-0.001616, 0.41454, 1.49987); // rotor
	modelPos[2] = Vector(0, 0.88784, -4.70182); // rudder
	modelPos[3] = Vector(0, 0.616756, -4.62009); // backwing_left
	modelPos[4] = Vector(0, 0.616756, -4.62009); // backwing_right
	modelPos[5] = Vector(2.82589, 0.119194, -0.946365); // wingflaps_left
	modelPos[6] = Vector(-2.82589, 0.119194,-0.946365); // wingflaps_right
}
Plane::~Plane()
{
	for (int i = 0; i < PLANE_MODEL_AMOUNT; i++) {
		delete models[i];
	}
}


/* Plane wird per spitfire.obj geladen
*  -> oeffnen .mtl per Editor und Pfade für die Texturen ändern 
* 
* Optimisierung: Nur die benötigten Teile der Texturen laden
*/ 
void Plane::loadModels(const char* path)
{
	std::string planePath = path;
	bool fitToSize = false;

	// plane
	this->model = new Model(&(planePath + "/spitfire.obj")[0], fitToSize);
	PhongShader* shader = new PhongShader();
	this->model->shader(shader, true);

	this->rotor = new Model(&(planePath + "/rotor.obj")[0], fitToSize);
	this->rotor->shader(shader, true);

	this->rudder = new Model(&(planePath + "/rudder.obj")[0], fitToSize);
	this->rudder->shader(shader, true);

	this->backwing_left = new Model(&(planePath + "/backwing_left.obj")[0], fitToSize);
	this->backwing_left->shader(shader, true);

	this->backwing_right = new Model(&(planePath + "/backwing_right.obj")[0], fitToSize);
	this->backwing_right->shader(shader, true);

	this->wingflaps_left = new Model(&(planePath + "/wingflaps_left.obj")[0], fitToSize);
	this->wingflaps_left->shader(shader, true);

	this->wingflaps_right = new Model(&(planePath + "/wingflaps_right.obj")[0], fitToSize);
	this->wingflaps_right->shader(shader, true);

	models[0] = this->model;
	models[1] = this->rotor;
	models[2] = this->rudder;
	models[3] = this->backwing_left;
	models[4] = this->backwing_right;
	models[5] = this->wingflaps_left;
	models[6] = this->wingflaps_right;

	for (int i = 0; i < PLANE_MODEL_AMOUNT; i++) {
		models[i]->transform(Matrix().translation(modelPos[i]));
	}
}

void Plane::draw(const BaseCamera& cam)
{
	for (int i = 0; i < PLANE_MODEL_AMOUNT; i++) {
		this->models[i]->draw(cam);
	}
}

void Plane::update(double delta)
{
	// rotation Z = roll
	// rotation Y = yaw 
	// rotation X = pitch
	Matrix forward, yaw, roll1, roll2, pitch;
	forward.translation(Vector(0, 0, 1 * speed * delta));
	this->yaw += 0.01f * -rudder_tilt * delta;
	yaw.rotationY(0.01f * -rudder_tilt * delta);
	pitch.rotationX(0.01f * -(left_flaps + right_flaps) * delta);
	this->pitch += 0.01f * -(left_flaps + right_flaps) * delta;

	roll1.rotationZ(0.01f * -(left_flaps) * delta);
	roll2.rotationZ(0.01f * (right_flaps) * delta);
	this->roll += 0.01f * -(left_flaps)*delta;
	this->roll += 0.01f * (right_flaps)*delta;

	this->model->transform(this->model->transform() * forward * yaw * roll1 * roll2 * pitch);
	this->transform(this->model->transform());

	// rudder tilt
	Matrix rudder_rot;
	rudder_rot.rotationY(1 * rudder_tilt * delta);
	this->rudder->transform(transform() * Matrix().translation(modelPos[2]) * rudder_rot);
	stabilizeToZero(this->rudder_tilt, 40);

	Matrix left_tilt_up, left_tilt_down, right_tilt_up, right_tilt_down;
	left_tilt_up.rotationX(4.11f * left_flaps * delta);
	left_tilt_down.rotationX(4.11f * abs(left_flaps) * delta);
	right_tilt_up.rotationX(4.11f * right_flaps * delta);
	std::cout << "left" << left_flaps << "right" << right_flaps << std::endl;


	this->backwing_left->transform(transform() * Matrix().translation(modelPos[3]) * left_tilt_up);
	this->wingflaps_left->transform(transform() * Matrix().translation(modelPos[5]) * left_tilt_up);
	this->wingflaps_right->transform(transform() * Matrix().translation(modelPos[6]) * right_tilt_up);
	this->backwing_right->transform(transform() * Matrix().translation(modelPos[4]) * right_tilt_up);
	stabilizeToZero(this->left_flaps, 40);
	stabilizeToZero(this->right_flaps, 40);


	// rotor
	Matrix rotor_rot;
	rotor_rot.rotationZ(speed * 25 * (rand() % 5));
	this->rotor->transform(transform() * Matrix().translation(modelPos[1]) * rotor_rot);

	// all flaps
}

void Plane::accelerate(float i)
{
	this->speed += i * DELTA_TIME_MULTIPLICATOR;

	
}

void Plane::leftFlapTilt(float i) {
	this->left_flaps += i * DELTA_TIME_MULTIPLICATOR * 30;
}

void Plane::rightFlapTilt(float i) {
	this->right_flaps += i * DELTA_TIME_MULTIPLICATOR * 30;
}

void Plane::rudderTilt(float i)
{
	this->rudder_tilt += i * DELTA_TIME_MULTIPLICATOR * 60;
}

