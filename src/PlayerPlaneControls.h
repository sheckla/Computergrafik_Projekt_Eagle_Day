#ifndef PlayerPlaneControls_h
#define PlayerPlaneControls_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Plane.h"
#include "Vector.h"

const Vector CAMERA_OFFSET(0.0f, 2.2f, -8.0f);

class PlayerPlaneControls
{
	GLFWwindow* window;
	Plane* plane;
	Camera* cam;
	bool follow = false;
	Matrix cameraPos;
	Matrix prevCameraPos;
	bool init = false;
public:
	PlayerPlaneControls(GLFWwindow* window, Plane* plane, Camera* cam, bool camFollowsPlane);
	void update(float delta);
};

#endif

