#ifndef PlayerPlaneControls_h
#define PlayerPlaneControls_h

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "Plane.h"

class PlayerPlaneControls
{
	GLFWwindow* window;
	Plane* plane;
	Camera* cam;
	bool followPlane = true;

public:
	PlayerPlaneControls(GLFWwindow* window, Plane* plane, Camera* cam);
	void update(float delta);
	float clamp(float upper, float lower, float& x);
};

#endif

