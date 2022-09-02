/*
 * Flugzeugkontrolle
 * - Tastatureingaben fuer die verschiedenen Steuerwinkel des Flugzeuges
 * - 'plane' muss im nachhinein an diese Steuereinheit angebunden werden
 */

#ifndef PlayerPlaneControls_h
#define PlayerPlaneControls_h

// Reihenfolge nicht verändern!
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// *

#include "Plane.h"
#include "Vector.h"
#include "irrKlang.h"

const Vector CAMERA_OFFSET(0.0f, 2.2f, -8.0f);

class PlayerPlaneControls
{
	GLFWwindow* window;
	Plane* plane;
	Camera* cam;
	bool follow = false;
	Matrix cameraPos;

	int i = 0;
public:
	PlayerPlaneControls(GLFWwindow* window, Plane* plane, Camera* cam, bool camFollowsPlane);
	void update(float delta);
	irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();;
};

#endif

