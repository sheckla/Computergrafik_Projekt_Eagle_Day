/*
* Plane
* - Containerklasse
* - Generiert mittels laden eines "plane.obj" ein Flugzeugmodell
*   mit mehreren Teilen
* 
* - Steuerung erfolgt �ber PlayerPlaneControls.h
* - Die verschiedenen Winkel der Flaps des Flugzeuges spielen in der Berechnung
*   des Flugwinkels eine Rolle
*/

#ifndef Plane_h
#define Plane_h

#include "BaseModel.h"
#include "Model.h"
#include "Camera.h"
#include "NetworkSender.h"
#include "ParticleLoader.h"
#include "irrKlang.h"
#include "TriangleSphereModel.h"

constexpr float EPSILON = 1e-7f; 
constexpr int PLANE_PARTS = 8;

// Translations Rotationswerte
constexpr float DELTA_TIME_MULTIPLICATOR = 1.0f; // Multiplikator f�r Drehgeschwindkeit, Beschleunigung etc.
constexpr float ACCELERATION_GAIN = 0.004f;
constexpr float ROTATION_SPEED = 0.0455f;
constexpr float MAX_TILT = 1.0f; // Tilt=[MAX_TILT, -MAX_TILT], Eingabe vom Nutzer
constexpr int MAX_SPEED = 600;

// Visuelle Rotationswerte zur Modelanpassung
constexpr float RUDDER_ROTATION = 1.0f; // visueller Neigungsfaktor
constexpr float FLAP_ROTATION = 1.6f; // visueller Neigungsfaktor 
constexpr float WINGFLAP_OFFSET_ROTATION = 0.35f; // fuer korrekte Rotation der Seiten-flaps

// Indices fuer parts
struct PartsIndex 
{
	static constexpr int model = 0;
	static constexpr int rotor = 1;
	static constexpr int rudder = 2;
	static constexpr int backWingLeft = 3;
	static constexpr int backWingRight = 4;
	static constexpr int wingLeft = 5;
	static constexpr int wingRight = 6;
	static constexpr int rotorBlur = 7;
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
	Vector rotorBlur = Vector(-0.001616f, 0.41454f, 1.49987f);
	const Vector OFFSETS[PLANE_PARTS]{model, rotor, rudder, backWingLeft, backWingRight, wingLeft, wingRight, rotorBlur};
	Model* parts[PLANE_PARTS];
	Matrix previousRotorRotation = Matrix().rotationZ(0);

	/*
	 * Orientierung = Hinter dem Flugzeug
	 * positive Werte = flaps werden nach 'oben' gekippt
	 * negative Werte = flaps werden nach 'unten' gekippt
	 */
	TiltStatus Tilt;// positiv = links, negativ = rechts
	Vector dotOffset = Vector(0, 0, 50);
	Vector horizonOffset = Vector(0, 0, 10);
	float speed = 0;

	// Engine Sounds
	irrklang::ISoundEngine* SoundEngine = nullptr;
	irrklang::ISoundEngine* HighPitchSoundEngine = nullptr;

	// Plane Bullet & Muzzle Particle Generators
	ParticleLoader* Gun_Left;
	ParticleLoader* Gun_Right;
	ParticleLoader* Muzzleflash_Right;
	ParticleLoader* Muzzleflash_Left;

	/*
	* Wert bleibt innerhalb [max_angle,-max_angle] und wird schrittweise
	* pro Aufruf gegen 0 angenaehert
	*/
	void aprroachZeroWithBoundaries(float& i, float maxAngle) const;

	// parts[i] wird mit der Haupt-Transformationsmatrix transfomiert
	void updateModelPos(const size_t index, const Matrix& transform) const;
	void clampTilt(float& i);
	bool loadModels(const char* path);
public:
	/*
	* Sendet infos ueber Internet
	*/
	bool isShooting = false;
	bool Online_Mode = false;

	ParticleLoader* Smoke_System;
	NetworkSender* Sender;
	TriangleSphereModel* dot; // Zielkreis
	float hp=100;
	float gunHP = 100;

	/* Plane wird per spitfire.obj geladen
	*  -> oeffne .mtl per Editor und Pfade f�r die Texturen �ndern
	*/
	Plane(const char* path);
	Plane(const char* path, const char* srv_Adr, int port);
	virtual ~Plane();
	void update(double delta);

	// parts werden mit zufaelligen Startpositionen transformiert (RESET)
	void initModelTranslation();

	// Setter (Steering)
	void accelerate(float i);
	void tiltLeftWingflaps(float i);
	void tiltRightWingflaps(float i);
	void tiltRudder(float i);

	// Getter
	Model** getParts();
	float getSpeed() const;
	const AABB& boundingBox() const;
	TiltStatus tilt();
	float speedPercentage() const; // aktueller Prozentanteil der maximalen Geschwindigkeit [0,1];
	Vector getPosition() { return Vector(parts[1]->transform().m03, parts[1]->transform().m13, parts[1]->transform().m23); }

	// Particles
	void startShooting();
	void stopShooting();
	void drawParticles(const BaseCamera& Cam);

	// (Sound)Engine enable/disable
	void startEngine();
	void stopEngine();
};

#endif

