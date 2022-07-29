/*
* Plane 
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

constexpr float EPSILON = 1e-4f; 
constexpr int PLANE_PARTS = 7;
constexpr float PI = 3.14159265359f;

// Konkrete Rotationswerte
constexpr int DELTA_TIME_MULTIPLICATOR = 60; // Multiplikator für Drehgeschwindkeit, Beschleunigung etc.
constexpr float SPEED_GAIN = 0.05f;
constexpr float ROTATION_SPEED = 0.02f;
constexpr float MAX_TILT = 30; // tilt=[MAX_TILT, -MAX_TILT], Eingabe vom Nutzer
constexpr int MAX_SPEED = 800;

// Visuelle Rotationswerte
constexpr float RUDDER_ROTATION = 2.0f; // visueller Neigungsfaktor
constexpr float FLAP_ROTATION = 3.0f; // visueller Neigungsfaktor 
constexpr float WINGFLAP_OFFSET_ROTATION = 0.0043f; // fuer korrekte Rotation der Seiten-flaps

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

class Plane : public BaseModel
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

	/*
	 * Orientierung = Hinter dem Flugzeug
	 * positive Werte = flaps werden nach 'oben' gekippt
	 * negative Werte = flaps werden nach 'unten' gekippt
	 */
	float leftFlapsTilt = 0; 
	float rightFlapsTilt = 0;
	float rudderTilt = 0; // positiv = links, negativ = rechts
	float speed = 0;
	Matrix previousRotorRotation = Matrix().rotationZ(0);

	/*
	* Wert bleibt innerhalb [max_angle,-max_angle] und wird schrittweise
	* pro Aufruf gegen 0 angenaehert
	*/
	void aprroachZeroWithBoundaries(float& i, float maxAngle) const;

	// aktueller Prozentanteil der maximalen Geschwindigkeit [0,1];
	float speedPercentage() const;

	void updateModelPos(const size_t index, const Matrix& transform) const;
	void clampTilt(float& i);
	bool loadModels(const char* path);

public:

	/* Plane wird per spitfire.obj geladen
	*  -> oeffne .mtl per Editor und Pfade für die Texturen ändern
	*
	* Optimisierung: Nur die benötigten Teile der Texturen laden
	*/
	Plane(const char* path);

	virtual ~Plane();
	void draw(const BaseCamera& cam) override;
	void update(double delta);

	void accelerate(float i);
	void tiltLeftWingflaps(float i);
	void tiltRightWingflaps(float i);
	void tiltRudder(float i);

	float getSpeed() const;
};

#endif

