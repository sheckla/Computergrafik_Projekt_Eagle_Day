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

Plane::Plane(const char* path, const char* srv_Adr, int port)
{
	print("loading plane", path);
	if (!loadModels(path))
	{
		throw "err";
	}
	Sender = new NetworkSender(srv_Adr, port);
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
	yaw.rotationY(ROTATION_SPEED * -Tilt.rudder * delta * speedMultiplier);
	pitch.rotationX(ROTATION_SPEED * -(Tilt.leftFlapsTilt + Tilt.rightFlapsTilt) * delta * speedMultiplier);
	rollLeft.rotationZ(ROTATION_SPEED * -Tilt.leftFlapsTilt * delta * speedMultiplier);
	rollRight.rotationZ(ROTATION_SPEED * Tilt.rightFlapsTilt * delta * speedMultiplier);

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
	rudderRotation.rotationY(RUDDER_ROTATION * Tilt.rudder * delta);
	updateModelPos(PartsIndex::rudder, rudderRotation);

	// left and right backwings
	Matrix leftBackwingRotation, rightBackwingRotation;
	leftBackwingRotation.rotationX(FLAP_ROTATION * Tilt.leftFlapsTilt * delta);
	rightBackwingRotation.rotationX(FLAP_ROTATION * Tilt.rightFlapsTilt * delta);
	updateModelPos(PartsIndex::backWingLeft, leftBackwingRotation);
	updateModelPos(PartsIndex::backWingRight, rightBackwingRotation);

	// left and right wingflaps
	// - wingflaps brauchen spezielle rotation damit sie korrekt aussehen
	Matrix leftWingRotation, rightWingRotation;
	leftWingRotation.rotationZ(Tilt.leftFlapsTilt * WINGFLAP_OFFSET_ROTATION);
	rightWingRotation.rotationZ(Tilt.rightFlapsTilt * -WINGFLAP_OFFSET_ROTATION);
	updateModelPos(PartsIndex::wingLeft, leftWingRotation * leftBackwingRotation);
	updateModelPos(PartsIndex::wingRight, rightWingRotation * rightBackwingRotation);

	// Fall-off fuer rudder & flaps gegen 0
	aprroachZeroWithBoundaries(Tilt.rudder, MAX_TILT);
	if (!d) aprroachZeroWithBoundaries(Tilt.leftFlapsTilt, MAX_TILT);
	aprroachZeroWithBoundaries(Tilt.rightFlapsTilt, MAX_TILT);

	// Geschwindigkeitsabfall TODO
	this->speed = speed * 0.999999;
	print("flap left", this->Tilt.leftFlapsTilt);
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
	Tilt.leftFlapsTilt += i * DELTA_TIME_MULTIPLICATOR;
	clampTilt(Tilt.leftFlapsTilt);
	if (abs(Tilt.leftFlapsTilt) == MAX_TILT)
	{
		d = true;
	} else
	{
		d = false;
	}
}

void Plane::tiltRightWingflaps(float i)
{
	Tilt.rightFlapsTilt += i * DELTA_TIME_MULTIPLICATOR;
	clampTilt(Tilt.leftFlapsTilt);
}

void Plane::tiltRudder(float i)
{
	Tilt.rudder += i * DELTA_TIME_MULTIPLICATOR;
	clampTilt(Tilt.rudder);
}

Model** Plane::getParts()
{
	return this->parts;
}

float Plane::getSpeed() const
{
	return this->speed;
}

TiltStatus Plane::tilt()
{
	return Tilt;
}
