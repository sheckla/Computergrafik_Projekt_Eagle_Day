#include "PlaneLoader.h"
#include "NetworkSender.h"

float Plane::speedPercentage()
{
	return (0 + (this->speed / 700));
}

void Plane::aprroachZeroWithBoundaries(float& i, float max_angle)
{
	// kleine werte werden zu 0
	if (abs(i) <= EPSILON) {
		i = 0;
		return;
	};

	// obere Grenze ueberschritten
	if (i > max_angle) {
		i = max_angle;
		return;
	}

	// untere Grenze uerberschritten
	if (i < -max_angle) {
		i = -max_angle;
		return;
	}

	// positiv; innerhalb Grenze
	if (i >= 0) {
		i = i * 0.99;
		return;
	}
	// negativ: innerhalb Grenze
	else if (i < 0) {
		i = i *0.99;
		return;
	}
}

void Plane::clampTilt(float& i)
{
	if (i > MAX_TILT) {
		i = MAX_TILT;
	}
	else if (i < -MAX_TILT) {
		i = -MAX_TILT;
	}
}

void Plane::updateModel(int i, Matrix& rotation)
{
	models[i]->transform(transform() * Matrix().translation(modelPos[i]) * rotation);
}

Plane::Plane(const char* srv_Adr, int port)
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

	Sender = new NetworkSender(srv_Adr, port);
}
Plane::~Plane()
{
	for (int i = 0; i < PLANE_MODEL_AMOUNT; i++) {
		delete models[i];
	}
}

void Plane::loadModels(const char* path)
{
	std::string planePath = path;
	bool fitToSize = false;

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

	models[1]->transform(Matrix().translation(modelPos[1]));
}

void Plane::draw(const BaseCamera& cam)
{
	for (int i = 0; i < PLANE_MODEL_AMOUNT; i++) {
		this->models[i]->draw(cam);
	}

}

/* 
* rotation Z = roll
* rotation Y = yaw 
* rotation X = pitch
*/ 
void Plane::update(double delta)
{

	// Konkrete Transformationen bezueglich Flugzeug- & Kameraposition
	Matrix forward, yaw, roll1, roll2, pitch;
	forward.translation(Vector(0, 0, SPEED_GAIN * speed * delta));

	if (this->speedPercentage() <= 0.1) {
		//forward.translation(Vector(0, -0.9768, 0) * delta);
	}

	yaw.rotationY(ROTATION_SPEED * -rudder_tilt * delta * speedPercentage());
	pitch.rotationX(ROTATION_SPEED * -(left_flaps_tilt + right_flaps_tilt) * delta * speedPercentage());
	roll1.rotationZ(ROTATION_SPEED * -(left_flaps_tilt) * delta * speedPercentage());
	roll2.rotationZ(ROTATION_SPEED * (right_flaps_tilt) * delta * speedPercentage());

	updateModel(0, forward * yaw * roll1 * roll2 * pitch);
	this->transform(this->model->transform());
	this->cameraPos = transform() * Matrix().translation(CAMERA_OFFSET);

	// Visuelle Transformationen
	// rotor 
	float pi = 3.14;
	Matrix rotor_rot;
	rotor_rot.rotationZ(pi * delta * speed * (rand() % 5));
	updateModel(1, rotor_rot);


	// rudder
	Matrix rudder_rot;
	rudder_rot.rotationY(RUDDER_ROTATION * rudder_tilt * delta);
	updateModel(2, rudder_rot);

	// left and right backwings
	Matrix left_flap_tilt_rot, right_flap_tilt_rot;
	left_flap_tilt_rot.rotationX(FLAP_ROTATION * left_flaps_tilt * delta);
	right_flap_tilt_rot.rotationX(FLAP_ROTATION * right_flaps_tilt * delta);
	updateModel(3, left_flap_tilt_rot);
	updateModel(4, right_flap_tilt_rot);

	// left and right wingflaps
	// - wingflaps brauchen spezielle rotation damit sie korrekt aussehen
	Matrix wingflap_left_rot, wingflap_right_rot;
	wingflap_left_rot.rotationZ(this->left_flaps_tilt * WINGFLAP_OFFSET_ROTATION);
	wingflap_right_rot.rotationZ(this->right_flaps_tilt * -WINGFLAP_OFFSET_ROTATION);

	updateModel(5, left_flap_tilt_rot * wingflap_left_rot);
	updateModel(6, right_flap_tilt_rot * wingflap_right_rot);


	/*print("flap left", this->left_flaps_tilt);
	print("flap right", this->right_flaps_tilt);
	print("speed", speedPercentage());
	print("speedval", this->speed);*/

	// Fall-off fuer rudder & flaps gegen 0
	aprroachZeroWithBoundaries(this->rudder_tilt, MAX_TILT);
	aprroachZeroWithBoundaries(this->left_flaps_tilt, MAX_TILT);
	aprroachZeroWithBoundaries(this->right_flaps_tilt, MAX_TILT);

	this->speed = speed * 0.999999;

}
// Spitfire max km/h = 594
void Plane::accelerate(float i)
{
	this->speed += i * DELTA_TIME_MULTIPLICATOR;

	if (this->speed >= 700) {
		this->speed = 700;
	}
	else if (this->speed < 0) {
		this->speed = 0;
	}
}

void Plane::leftFlapTilt(float i) {
	this->left_flaps_tilt += i * DELTA_TIME_MULTIPLICATOR;
	clampTilt(this->left_flaps_tilt);
}

void Plane::rightFlapTilt(float i) {
	this->right_flaps_tilt += i * DELTA_TIME_MULTIPLICATOR;
	clampTilt(this->right_flaps_tilt);
}

void Plane::rudderTilt(float i)
{
	this->rudder_tilt += i * DELTA_TIME_MULTIPLICATOR;
	clampTilt(this->rudder_tilt);
}

