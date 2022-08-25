#include "PlayerPlaneControls.h"

#include "MouseLogger.h"


PlayerPlaneControls::PlayerPlaneControls(GLFWwindow* window, Plane* plane, Camera* cam, bool camFollowsPlane) :
    window(window), plane(plane), cam(cam), follow(camFollowsPlane)
{
}

void PlayerPlaneControls::update(float delta) 
{
    float yOffset = (plane->tilt().leftFlapsTilt + plane->tilt().rightFlapsTilt) * plane->speedPercentage();
    float xOffset = (-plane->tilt().rudder) * plane->speedPercentage();
    Vector offset = CAMERA_OFFSET + Vector(xOffset, yOffset, 0);
    this->cameraPos = Matrix().translation(Vector(0,2.2,0)) * plane->getParts()[0]->transform() * Matrix().translation(offset);// * Matrix().translation(CAMERA_OFFSET);
    this->cameraPos =  plane->getParts()[0]->transform() * Matrix().translation(offset);// * Matrix().translation(CAMERA_OFFSET);


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
        plane->tiltLeftWingflaps(delta);
        plane->tiltRightWingflaps(delta);
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



    ///////////////////////////////////////////////////////////////////////////////TEST//////////
    /*
    if (glfwGetKey(this->window, GLFW_KEY_T))
    {
        plane->Smoke_System->StartGenerating();
        std::cout << "[PlayerPlaneControls] Start Particles" << std::endl;
    }

    if (glfwGetKey(this->window, GLFW_KEY_G))
    {
        plane->Smoke_System->StopGenerating();
        std::cout << "[PlayerPlaneControls] Stop Particles" << std::endl;
    }
    */
    ///////////////////////////////////////////////////////////////////////////////TEST//////////

    if (glfwGetKey(this->window, GLFW_KEY_SPACE))
    {
        plane->Gun_Left->StartGenerating();
        plane->Gun_Right->StartGenerating();
        plane->isShooting = true;
    }
    else
    {
        plane->Gun_Left->StopGenerating();
        plane->Gun_Right->StopGenerating();
        plane->isShooting = false;
    }
    

    MouseLogger logger = MouseLogger::instance();
    float normX = ((float)logger.x() / ASPECT_WIDTH * 2) - 1;
    float normY = ((float)logger.y() / ASPECT_HEIGHT * 2) - 1;
    float up = -normY / 2;
    float leftTilt, rightTilt;
    leftTilt = 0;
    rightTilt = 0;

    // Rolle Rechts
    if (normX > 0) {
        leftTilt = normY * normX;
        rightTilt = -normY * normX;
    }
    // Rolle links
    if (normX < 0) {
        leftTilt = normY * normX;
        rightTilt = -normY * normX;
    }
    // Mouse Controls
	//plane->tiltRudder(delta * normX);
    //plane->tiltLeftWingflaps(delta*2 * (up + leftTilt));
    //plane->tiltRightWingflaps(delta*2 * (up + rightTilt));


    plane->update(delta);


    // camera follows plane
    cam->setTarget(plane->getParts()[0]->transform().translation());
    cam->setPosition(cameraPos.translation());
    cam->zoom(-plane->getSpeed() / 60);
}
