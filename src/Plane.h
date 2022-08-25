/*
* Plane
* - Containerklasse
* - Generiert mittels laden des eines "plane.obj" ein Flugzeugmodell
*   mit mehreren Teilen
* 
* - Steuerung erfolgt über PlayerPlaneControls.h
* - Die verschiedenen Winkel der Flaps des Flugzeuges spielen in der Berechnung
*   des Flugwinkels eine Rolle
*/

#ifndef Plane_h
#define Plane_h

#include "BaseModel.h"
#include "Model.h"
#include "Camera.h"
#include "PhongShader.h"
#include <stdlib.h>
#include "NetworkSender.h"
#include "TriangleBoxModel.h"
#include "ParticleLoader.h"

class TriangleSphereModel;
constexpr float EPSILON = 1e-7f; 
constexpr int PLANE_PARTS = 7;
constexpr float PI = 3.14159265359f;

// Konkrete Rotationswerte
constexpr int DELTA_TIME_MULTIPLICATOR = 1.0f; // Multiplikator für Drehgeschwindkeit, Beschleunigung etc.
constexpr float ACCELERATION_GAIN = 0.002f;
constexpr float ROTATION_SPEED = .0115f;
constexpr float MAX_TILT = 1.0f; // Tilt=[MAX_TILT, -MAX_TILT], Eingabe vom Nutzer
constexpr int MAX_SPEED = 556;

// Visuelle Rotationswerte
constexpr float RUDDER_ROTATION = 1.0f; // visueller Neigungsfaktor
constexpr float FLAP_ROTATION = 1.3f; // visueller Neigungsfaktor 
constexpr float WINGFLAP_OFFSET_ROTATION = 0.35f; // fuer korrekte Rotation der Seiten-flaps

struct PartsIndex 
{
	static constexpr int model = 0;
	static constexpr int rotor = 1;
	static constexpr int rudder = 2;
	static constexpr int backWingLeft = 3;
	static constexpr int backWingRight = 4;
	static constexpr int wingLeft = 5;
	static constexpr int wingRight = 6;
};

struct TiltStatus
{
	float leftFlapsTilt = 0;
	float rightFlapsTilt = 0;
	float rudder = 0;
};


class Plane
{
	/*
	* Positions-offsets
	* Blender Y = -Z
	* Blender Z = Y
	*/ 
	Vector model = Vector(0, 0, 0);
	Vector rotor = Vector(-0.001616f, 0.41454f, 1.49987f);
	Vector rudder = Vector(0, 0.88784f, -4.70182f);
	Vector backWingLeft = Vector(0, 0.616756f, -4.62009f);
	Vector backWingRight = Vector(0, 0.616756f, -4.62009f);
	Vector wingLeft = Vector(2.82589f, 0.119194f, -0.946365f);
	Vector wingRight = Vector(-2.82589f, 0.119194f, -0.946365f);
	const Vector OFFSETS[PLANE_PARTS]{model, rotor, rudder, backWingLeft, backWingRight, wingLeft, wingRight};
	Model* parts[PLANE_PARTS];
	Matrix previousRotorRotation = Matrix().rotationZ(0);

	/*
	 * Orientierung = Hinter dem Flugzeug
	 * positive Werte = flaps werden nach 'oben' gekippt
	 * negative Werte = flaps werden nach 'unten' gekippt
	 */
	TiltStatus Tilt;// positiv = links, negativ = rechts
	float speed = 0;
	Vector dotOffset = Vector(0, 0, 50);
	Vector horizonOffset = Vector(0, 0, 10);

	/*
	* Wert bleibt innerhalb [max_angle,-max_angle] und wird schrittweise
	* pro Aufruf gegen 0 angenaehert
	*/
	void aprroachZeroWithBoundaries(float& i, float maxAngle) const;



	void updateModelPos(const size_t index, const Matrix& transform) const;
	void clampTilt(float& i);
	bool loadModels(const char* path);

public:
	TriangleSphereModel* dot;
	TriangleBoxModel* horizon;

	bool isShooting = false;

	/* Plane wird per spitfire.obj geladen
	*  -> oeffne .mtl per Editor und Pfade für die Texturen ändern
	*/
	Plane(const char* path);
	Plane(const char* path, const char* srv_Adr, int port);
	virtual ~Plane();
	void update(double delta);

	// Setter
	void accelerate(float i);
	void tiltLeftWingflaps(float i);
	void tiltRightWingflaps(float i);
	void tiltRudder(float i);

	// Getter
	Model** getParts();
	float getSpeed() const;
	TiltStatus tilt();

	// aktueller Prozentanteil der maximalen Geschwindigkeit [0,1];
	float speedPercentage() const;
	Vector getPosition() { return Vector(parts[1]->transform().m03, parts[1]->transform().m13, parts[1]->transform().m23); }

	void drawParticles(const BaseCamera& Cam) { 
		this->Smoke_System->draw(Cam);
		this->Gun_Left->draw(Cam);
		this->Gun_Right->draw(Cam);
	}

	/*
	* Sendet infos ueber Internet
	*/
	NetworkSender* Sender;
	bool Online_Mode = false;

	/**
	* Rauch, kann mit T und G ein- und ausgeschaltet werden (Bis HP existiert)
	*/
	ParticleLoader* Smoke_System;
	ParticleLoader* Gun_Left;
	ParticleLoader* Gun_Right;

	float hp=100;
};

#endif

