#include "PlayerPlaneControls.h"


PlayerPlaneControls::PlayerPlaneControls(GLFWwindow* window, Plane* plane, Camera* cam, bool camFollowsPlane) :
    window(window), plane(plane), cam(cam), follow(camFollowsPlane)
{
}

void PlayerPlaneControls::update(float delta) 
{
    Vector offset = CAMERA_OFFSET + Vector(0, 0*(plane->tilt().leftFlapsTilt + plane->tilt().rightFlapsTilt) * delta, 0);
    this->cameraPos = plane->getParts()[0]->transform() * Matrix().translation(offset);// * Matrix().translation(CAMERA_OFFSET);

    // Bei maximalen 'Tilt' werden die eingaben ignorieren, Plane hat schon maximale Wendung
    plane->update(delta);

    // Beschleunigung
    if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT))
    {
        plane->accelerate(delta);
    }

    // Abbremsen
    if (glfwGetKey(this->window, GLFW_KEY_LEFT_CONTROL))
    {
        plane->accelerate(-delta);
    }

    // Rolle (links)
    if (glfwGetKey(this->window, GLFW_KEY_A))
    {
        plane->tiltLeftWingflaps(delta);
        plane->tiltRightWingflaps(-delta);
    }

    // Rolle (rechts)
    if (glfwGetKey(this->window, GLFW_KEY_D))
    {
        plane->tiltLeftWingflaps(-delta);
        plane->tiltRightWingflaps(delta);
    }

    // Aufstieg
    if (glfwGetKey(this->window, GLFW_KEY_S))
    {
        if (!plane->d) plane->tiltLeftWingflaps(delta);
        if (abs(plane->tilt().rightFlapsTilt) < MAX_TILT) plane->tiltRightWingflaps(delta);
    }

    // Absturz
    if (glfwGetKey(this->window, GLFW_KEY_W))
    {
        plane->tiltLeftWingflaps(-delta);
        plane->tiltRightWingflaps(-delta);
    }

    // Wenden (links)
    if (glfwGetKey(this->window, GLFW_KEY_Q))
    {
        plane->tiltRudder(-delta);
    }

    // Wenden (rechts)
    if (glfwGetKey(this->window, GLFW_KEY_E))
    {
        plane->tiltRudder(delta);
    }


    // camera follows plane
    cam->setTarget(plane->getParts()[0]->transform().translation());
    cam->setPosition(cameraPos.translation());
    //cam->zoom(-plane->getSpeed() / 60);
}
