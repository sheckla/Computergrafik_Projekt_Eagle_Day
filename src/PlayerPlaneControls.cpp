#include "PlayerPlaneControls.h"


PlayerPlaneControls::PlayerPlaneControls(GLFWwindow* window, Plane* plane, Camera* cam) :
    window(window), plane(plane), cam(cam)
{
}

void PlayerPlaneControls::update(float delta) {
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
        plane->leftFlapTilt(delta);
        plane->rightFlapTilt(-delta);
    }

    // Rolle (rechts)
    if (glfwGetKey(this->window, GLFW_KEY_D))
    {
        plane->leftFlapTilt(-delta);
        plane->rightFlapTilt(delta);
    }

    // Aufstieg
    if (glfwGetKey(this->window, GLFW_KEY_S))
    {
        plane->leftFlapTilt(delta);
        plane->rightFlapTilt(delta);
    }

    // Absturz
    if (glfwGetKey(this->window, GLFW_KEY_W))
    {
        plane->leftFlapTilt(-delta);
        plane->rightFlapTilt(-delta);
    }

    // Wenden (links)
    if (glfwGetKey(this->window, GLFW_KEY_Q))
    {
        plane->rudderTilt(-delta);
    }

    // Wenden (rechts)
    if (glfwGetKey(this->window, GLFW_KEY_E))
    {
        plane->rudderTilt(delta);
    }

    plane->update(delta);

    // camera follows plane
    cam->setTarget(plane->transform().translation());
    cam->setPosition(plane->cameraPos.translation());
    cam->zoom(-plane->speed/100);
}