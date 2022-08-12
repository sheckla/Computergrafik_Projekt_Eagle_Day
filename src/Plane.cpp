#include "PlaneLoader.h"
#include "Printer.h"
#include "NetworkSender.h"

float Plane::speedPercentage() const
{
	return (0 + (this->speed / MAX_SPEED));
}

void Plane::updateModelPos(const size_t index, const Matrix& transform) const
{
	parts[index]->transform( parts[0]->transform() * Matrix().translation(OFFSETS[index]) * transform);
}

void Plane::aprroachZeroWithBoundaries(float& i, const float maxAngle) const
{
	// kleine werte werden zu 0
	if (abs(i) <= EPSILON)
	{
		i = 0;
		return;
	}

	// obere Grenze ueberschritten
	if (i > maxAngle)
	{
		i = maxAngle;
		return;
	}

	// untere Grenze uerberschritten
	if (i < -maxAngle)
	{
		i = -maxAngle;
		return;
	}

	// positiv; innerhalb Grenze
	if (i >= 0)
	{
		i = i * 0.99;
		return;
	}
	// negativ: innerhalb Grenze
	else if (i < 0)
	{
		i = i * 0.99;
		return;
	}
}

void Plane::clampTilt(float& i)
{
	if (i > MAX_TILT)
	{
		i = MAX_TILT;
	}
	else if (i < -MAX_TILT)
	{
		i = -MAX_TILT;
	}
}

Plane::Plane(const char* path)
{
	print("loading Plane", path);
	if (!loadModels(path))
	{
		throw "err";
	}
}

Plane::Plane(const char* srv_Adr, int port)
{
	loadModels(ASSETS "models/spitfire");
	//Sender = new NetworkSender(srv_Adr, port);
}

Plane::~Plane()
{
	for (size_t i = 0; i < PLANE_PARTS; i++)
	{
		delete parts[i];
	}
}

bool Plane::loadModels(const char* path)
{
	const std::string planePath = path;
	bool fitToSize = false;
	PhongShader* shader = new PhongShader();


	// Modelle Laden
	parts[PartsIndex::model] = new Model(&(planePath + "/spitfire.obj")[0], fitToSize);
	parts[PartsIndex::model]->shader(shader, true);

	parts[PartsIndex::rotor] = new Model(&(planePath + "/rotor.obj")[0], fitToSize);
	parts[PartsIndex::rotor]->shader(shader, true);

	parts[PartsIndex::rudder] = new Model(&(planePath + "/rudder.obj")[0], fitToSize);
	parts[PartsIndex::rudder]->shader(shader, true);

	parts[PartsIndex::backWingLeft] = new Model(&(planePath + "/backwing_left.obj")[0], fitToSize);
	parts[PartsIndex::backWingLeft]->shader(shader, true);

	parts[PartsIndex::backWingRight] = new Model(&(planePath + "/backwing_right.obj")[0], fitToSize);
	parts[PartsIndex::backWingRight]->shader(shader, true);

	parts[PartsIndex::wingLeft] = new Model(&(planePath + "/wingflaps_left.obj")[0], fitToSize);
	parts[PartsIndex::wingLeft]->shader(shader, true);

	parts[PartsIndex::wingRight] = new Model(&(planePath + "/wingflaps_right.obj")[0], fitToSize);
	parts[PartsIndex::wingRight]->shader(shader, true);

	// Offsets anwenden
	for (size_t i = 0; i < PLANE_PARTS; i++)
	{
		parts[i]->transform(Matrix().translation(OFFSETS[i]));
	}
	return true;
}

/* 
* rotation Z = roll
* rotation Y = yaw 
* rotation X = pitch
*/
void Plane::update(double delta)
{
	// Konkrete Transformationen bezueglich Flugzeug- & Kameraposition
	Matrix forward, yaw, rollLeft, rollRight, pitch;
	const float speedMultiplier = speedPercentage();

	forward.translation(Vector(0, 0, SPEED_GAIN * speed * delta));
	yaw.rotationY(ROTATION_SPEED * -rudderTilt * delta * speedMultiplier);
	pitch.rotationX(ROTATION_SPEED * -(leftFlapsTilt + rightFlapsTilt) * delta * speedMultiplier);
	rollLeft.rotationZ(ROTATION_SPEED * -leftFlapsTilt * delta * speedMultiplier);
	rollRight.rotationZ(ROTATION_SPEED * rightFlapsTilt * delta * speedMultiplier);

	// Auf objekt anwenden
	//this->transform(transform() * forward * yaw * pitch * rollLeft * rollRight);

	// main-model
	parts[0]->transform(parts[0]->transform() * forward * yaw * pitch * rollLeft * rollRight);
	
	// Visuelle Transformationen
	// rotor
	Matrix rotorRotation;
	rotorRotation.rotationZ(PI * delta * speed);
	updateModelPos(PartsIndex::rotor, rotorRotation * previousRotorRotation);
	previousRotorRotation = rotorRotation * previousRotorRotation;

	// rudder
	Matrix rudderRotation;
	rudderRotation.rotationY(RUDDER_ROTATION * rudderTilt * delta);
	updateModelPos(PartsIndex::rudder, rudderRotation);

	// left and right backwings
	Matrix leftBackwingRotation, rightBackwingRotation;
	leftBackwingRotation.rotationX(FLAP_ROTATION * leftFlapsTilt * delta);
	rightBackwingRotation.rotationX(FLAP_ROTATION * rightFlapsTilt * delta);
	updateModelPos(PartsIndex::backWingLeft, leftBackwingRotation);
	updateModelPos(PartsIndex::backWingRight, rightBackwingRotation);

	// left and right wingflaps
	// - wingflaps brauchen spezielle rotation damit sie korrekt aussehen
	Matrix leftWingRotation, rightWingRotation;
	leftWingRotation.rotationZ(this->leftFlapsTilt * WINGFLAP_OFFSET_ROTATION);
	rightWingRotation.rotationZ(this->rightFlapsTilt * -WINGFLAP_OFFSET_ROTATION);
	updateModelPos(PartsIndex::wingLeft, leftWingRotation * leftBackwingRotation);
	updateModelPos(PartsIndex::wingRight, rightWingRotation * rightBackwingRotation);


	// Fall-off fuer rudder & flaps gegen 0
	aprroachZeroWithBoundaries(this->rudderTilt, MAX_TILT);
	aprroachZeroWithBoundaries(this->leftFlapsTilt, MAX_TILT);
	aprroachZeroWithBoundaries(this->rightFlapsTilt, MAX_TILT);

	this->speed = speed * 0.999999;
	/*print("flap right", this->rightFlapsTilt);
	print("flap left", this->leftFlapsTilt);
	print("speed", speedPercentage());
	print("speedval", this->speed);*/
}

// Spitfire max km/h = 594
void Plane::accelerate(float i)
{
	this->speed += i * DELTA_TIME_MULTIPLICATOR;

	if (this->speed >= 700)
	{
		this->speed = 700;
	}
	else if (this->speed < 0)
	{
		this->speed = 0;
	}
}

void Plane::tiltLeftWingflaps(float i)
{
	this->leftFlapsTilt += i * DELTA_TIME_MULTIPLICATOR;
	clampTilt(this->leftFlapsTilt);
}

void Plane::tiltRightWingflaps(float i)
{
	this->rightFlapsTilt += i * DELTA_TIME_MULTIPLICATOR;
	clampTilt(this->rightFlapsTilt);
}

void Plane::tiltRudder(float i)
{
	this->rudderTilt += i * DELTA_TIME_MULTIPLICATOR;
	clampTilt(this->rudderTilt);
}

Model** Plane::getParts()
{
	return this->parts;
}

float Plane::getSpeed() const
{
	return this->speed;
}
