#include <irrKlang.h>

#include "ApplicationSettings.h"
#include "constantshader.h"
#include "MathUtil.h"
#include "PlaneLoader.h"
#include "Printer.h"
#include "NetworkSender.h"
#include "TriangleSphereModel.h"
#include "CollisionDetector.h"

float Plane::speedPercentage() const
{
	return (0 + (this->speed / (float)MAX_SPEED));
}

void Plane::startEngine()
{
	SoundEngine = irrklang::createIrrKlangDevice();
	SoundEngine->setSoundVolume(0);
	SoundEngine->play2D(ASSETS "audio/steady.wav", true);

	HighPitchSoundEngine = irrklang::createIrrKlangDevice();
	HighPitchSoundEngine->setSoundVolume(0);
	HighPitchSoundEngine->play2D(ASSETS "audio/steady_high_pitch.wav", true);
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

	// positiv; innerhalb Grenze
	if (i >= 0)
	{
		i = i * 0.90;
		return;
	}
	// negativ: innerhalb Grenze
	if (i < 0)
	{
		i = i * 0.90;
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

	Smoke_System = new ParticleLoader(.02, .5, ParticleType::Smoke);

	Gun_Left = new ParticleLoader(.01, 2, ParticleType::Bullet);
	Gun_Left->setOffset(-2.5f);
	Muzzleflash_Left = new ParticleLoader(.01, .03, ParticleType::MuzzleFlash);
	Muzzleflash_Left->setOffset(-2.5f);


	Gun_Right = new ParticleLoader(.01, 2, ParticleType::Bullet);
	Gun_Right->setOffset(2.5f);
	Muzzleflash_Right = new ParticleLoader(.01, .03, ParticleType::MuzzleFlash);
	Muzzleflash_Right->setOffset(2.5f);
}

Plane::Plane(const char* path, const char* srv_Adr, int port) : Plane(path) // <- Calls normal constructor
{
	Sender = new NetworkSender(srv_Adr, port);
	Online_Mode = true;
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
	dot = new TriangleSphereModel(0.1, 20, 20);
	dot->shader(new ConstantShader());
	dot->shadowCaster(false);
	dot->transform(Matrix().translation(dotOffset));

	horizon = new TriangleBoxModel(20, 1, 1);
	horizon->shader(new ConstantShader());
	horizon->shadowCaster(false);
	//horizon->transform(Matrix().translation(horizonOffset));
	return true;
}

/* 
* rotation Z = roll
* rotation Y = yaw 
* rotation X = pitch
*/
void Plane::update(double delta)
{
	// Translations/Rotations Transformationen bezueglich Flugzeug- & Kameraposition
	Matrix forward, yaw, rollLeft, rollRight, pitch;
	const float speedMultiplier = speedPercentage();

	forward.translation(Vector(0, 0, ACCELERATION_GAIN * speed));
	yaw.rotationY(ROTATION_SPEED * -Tilt.rudder * speedMultiplier);
	pitch.rotationX(ROTATION_SPEED * -(Tilt.leftFlapsTilt + Tilt.rightFlapsTilt) * speedMultiplier);
	rollLeft.rotationZ(ROTATION_SPEED * -Tilt.leftFlapsTilt * speedMultiplier);
	rollRight.rotationZ(ROTATION_SPEED * Tilt.rightFlapsTilt * speedMultiplier);

	// main-model
	parts[0]->transform(parts[0]->transform() * forward * yaw * pitch * rollLeft * rollRight);
	dot->transform(parts[0]->transform() * Matrix().translation(dotOffset));

	Matrix rollLeftInvert, rollRightInvert;
	rollLeftInvert.rotationZ(-(ROTATION_SPEED * -Tilt.leftFlapsTilt * speedMultiplier));
	rollRightInvert.rotationZ(-(ROTATION_SPEED * Tilt.rightFlapsTilt * speedMultiplier));
	horizon->transform(parts[0]->transform() * rollLeftInvert * Matrix().translation(horizonOffset));

	// Visuelle Transformationen
	// rotor
	Matrix rotorRotation;
	rotorRotation.rotationZ(PI * delta * speed * 5);
	updateModelPos(PartsIndex::rotor, rotorRotation * previousRotorRotation);
	previousRotorRotation = rotorRotation * previousRotorRotation;

	// rudder
	Matrix rudderRotation;
	rudderRotation.rotationY(RUDDER_ROTATION * Tilt.rudder);
	updateModelPos(PartsIndex::rudder, rudderRotation);

	// left and right backwings
	Matrix leftBackwingRotation, rightBackwingRotation;
	leftBackwingRotation.rotationX(FLAP_ROTATION * Tilt.leftFlapsTilt );
	rightBackwingRotation.rotationX(FLAP_ROTATION * Tilt.rightFlapsTilt );
	updateModelPos(PartsIndex::backWingLeft, leftBackwingRotation);
	updateModelPos(PartsIndex::backWingRight, rightBackwingRotation);

	// visuelle rotation
	Matrix leftWingRotation, rightWingRotation;
	leftWingRotation.rotationZ(Tilt.leftFlapsTilt * WINGFLAP_OFFSET_ROTATION);
	rightWingRotation.rotationZ(Tilt.rightFlapsTilt * -WINGFLAP_OFFSET_ROTATION);
	updateModelPos(PartsIndex::wingRight, rightWingRotation * rightBackwingRotation);
	updateModelPos(PartsIndex::wingLeft, leftWingRotation * leftBackwingRotation);

	// left and right wingflaps
	// - wingflaps brauchen spezielle rotation damit sie korrekt aussehen

	// Fall-off fuer rudder & flaps gegen 0
	aprroachZeroWithBoundaries(Tilt.rudder, MAX_TILT);
	aprroachZeroWithBoundaries(Tilt.leftFlapsTilt, MAX_TILT);
	aprroachZeroWithBoundaries(Tilt.rightFlapsTilt, MAX_TILT);

	// Geschwindigkeitsabfall TODO
	this->speed = speed - (5*delta * speedPercentage());
	/*print("flap right", this->rightFlapsTilt);
	print("flap left", this->leftFlapsTilt);
	print("speed", speedPercentage());
	print("speedval", this->speed);*/


	if(this->hp < 50.0f)this->Smoke_System->StartGenerating();

	Smoke_System->update(delta, this->parts[1]->transform());
	Gun_Left->update(delta, this->parts[0]->transform()); 
	Gun_Right->update(delta, this->parts[0]->transform());

	Muzzleflash_Right->update(delta, this->parts[0]->transform());
	Muzzleflash_Left->update(delta, this->parts[0]->transform());

	if (Online_Mode)Sender->SendData(this);

	if (SoundEngine && HighPitchSoundEngine)
	{
		float steady =  1 - speedPercentage();
		float high = speedPercentage();

		SoundEngine->setSoundVolume((0.3 + speedPercentage()) * ApplicationSettings::AUDIO_VALUE);
		if (speedPercentage() > 0.55)
		{
			HighPitchSoundEngine->setSoundVolume(MathUtil::remapBounds(speedPercentage(), 0.55, 1, 0, 1) * ApplicationSettings::AUDIO_VALUE);
			
		} else
		{
			HighPitchSoundEngine->setSoundVolume(0);
		}
	}

	//Hard Collisions
	if (CollisionDetector::CheckPlaneCollision(this->boundingBox())) { this->hp = -1; }
}

// Spitfire max km/h = 594
void Plane::accelerate(float i)
{
	this->speed += i * DELTA_TIME_MULTIPLICATOR * 80;

	if (this->speed >= MAX_SPEED)
	{
		this->speed = MAX_SPEED;
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
}

void Plane::tiltRightWingflaps(float i)
{
	Tilt.rightFlapsTilt += i * DELTA_TIME_MULTIPLICATOR;
	clampTilt(Tilt.rightFlapsTilt);
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
