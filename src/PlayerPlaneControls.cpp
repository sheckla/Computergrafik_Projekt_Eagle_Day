#include "PlayerPlaneControls.h"


PlayerPlaneControls::PlayerPlaneControls(GLFWwindow* window, Plane* plane, Camera* cam, bool camFollowsPlane) :
    window(window), plane(plane), cam(cam), follow(camFollowsPlane)
{
}

static Vector lerp(const Vector& A, const Vector& B, float t) {
    return A * t + B * (1.f - t);
}

void PlayerPlaneControls::update(float delta) 
{

    this->cameraPos = plane->getParts()[0]->transform() * Matrix().translation(CAMERA_OFFSET);


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


    if (glfwGetKey(this->window, GLFW_KEY_SPACE))
    {
        plane->Gun_Left->StartGenerating();
        plane->Gun_Right->StartGenerating();
        //std::cout << "[PlayerPlaneControls] Start Gun Particles" << std::endl;
    }
    else
    {
        plane->Gun_Left->StopGenerating();
        plane->Gun_Right->StopGenerating();
        //std::cout << "[PlayerPlaneControls] Stop Gun Particles" << std::endl;
    }
    ///////////////////////////////////////////////////////////////////////////////TEST//////////

    plane->update(delta);

    // camera follows plane
    if (follow) {
        cam->setTarget(plane->getParts()[0]->transform().translation());
        //print("prev", prevCameraPos.translation());
        //print("current", cameraPos.translation());
        //print("lerp", lerp(prevCameraPos.translation(), cameraPos.translation(), 0.5));
        cam->setPosition(prevCameraPos.translation());
        cam->zoom(-plane->getSpeed() / 60);

        if (init)
        {
            //print("double", "");
        }
    	else
        {
            //print("once", "");
            cam->setPosition(cameraPos.translation());
			this->init = true;
        }
			prevCameraPos = cameraPos;
    }
}
