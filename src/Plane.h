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

#include "Globals.h"
#include "basemodel.h"
#include "Model.h"
#include "Camera.h"
#include "PhongShader.h"
#include <stdlib.h>
#include "NetworkSender.h"

const float EPSILON = 1e-4; 
const int PLANE_MODEL_AMOUNT = 7;
const Vector CAMERA_OFFSET(0, 2.2, -8); // Offset für die statische Kamera hinter dem Flugzeug

// Konkrete Rotationswerte
const int DELTA_TIME_MULTIPLICATOR = 60; // Multiplikator für Drehgeschwindkeit, Beschleunigung etc.
const float SPEED_GAIN = 0.05;
const float ROTATION_SPEED = 0.02f;
const float MAX_TILT = 30; // tilt=[MAX_TILT, -MAX_TILT], Eingabe vom Nutzer

// Visuelle Rotationswerte
const float RUDDER_ROTATION = 2.0f; // visueller Neigungsfaktor
const float FLAP_ROTATION = 3.0f; // visueller Neigungsfaktor 
const float WINGFLAP_OFFSET_ROTATION = 0.0043; // fuer korrekte Rotation der Seiten-flaps

class Plane : public BaseModel
{
    Vector modelPos[PLANE_MODEL_AMOUNT]; // Offsets fuer die plane-parts
	Model* models[PLANE_MODEL_AMOUNT];
	Model* model; // 0
	Model* rotor; // 1
	Model* rudder; // 2
	Model* backwing_left; // 3
	Model* backwing_right; // 4
	Model* wingflaps_left; // 5
	Model* wingflaps_right; // 6

	float rudder_tilt = 0; // X<0 = left, X>0 = right
	float left_flaps_tilt = 0; // X>0 = up, X<0 = down 
	float right_flaps_tilt = 0; // X>0 = up, X<0 = down 
	float speedPercentage();
	/*
	* Wert bleibt innerhalb [max_angle,-max_angle] und wird schrittweise
	* pro Aufruf gegen 0 angenaehert
	*/
	void aprroachZeroWithBoundaries(float& i, float max_angle);
	void clampTilt(float& i);
	void updateModel(int i, Matrix& rotation);
public:
	Matrix cameraPos;
	float speed = 0;

	/* Plane wird per spitfire.obj geladen
	*  -> oeffne .mtl per Editor und Pfade für die Texturen ändern
	*
	* Optimisierung: Nur die benötigten Teile der Texturen laden
	*/
	Plane(const char* srv_Adr, int port);

	virtual ~Plane();
	void loadModels(const char* path);
	virtual void draw(const BaseCamera& cam);
	void update(double delta);
	void accelerate(float i);
	void leftFlapTilt(float i);
	void rightFlapTilt(float i);
	void rudderTilt(float i);

	NetworkSender* Sender;
};

#endif

