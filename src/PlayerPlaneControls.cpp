#include "PlayerPlaneControls.h"

#include "ApplicationGUI.h"
#include "ApplicationSettings.h"
#include "MathUtil.h"
#include "MouseLogger.h"


PlayerPlaneControls::PlayerPlaneControls(GLFWwindow* window, Plane* plane, Camera* cam, bool camFollowsPlane) :
    window(window), plane(plane), cam(cam), follow(camFollowsPlane)
{
}

void PlayerPlaneControls::update(float delta) 
{
    // Camera offset according to plane rotation angle
    float yOffset = (plane->tilt().leftFlapsTilt + plane->tilt().rightFlapsTilt) * plane->speedPercentage() * 2;
    float xOffset = (-plane->tilt().rudder) * plane->speedPercentage() * 2;
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

    // Plane Shooting
    if (glfwGetKey(this->window, GLFW_KEY_SPACE))
    {
        plane->startShooting();

        // PostProcessing Shake Effect
        ApplicationGUI::AppGUI->ppBuffer->shake(true);
        ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(true);
        ApplicationGUI::AppGUI->ppBuffer->gaussianBlur(false);

        // Audio effects
        ShootSoundEngine->setSoundVolume(ApplicationSettings::AUDIO_VALUE);
        plane->gunHP--; // Lower Gun Health

        // Gun Health < 0 ? -> decrease Plane HP
        if (plane->gunHP <= 0)
        {
            plane->hp -= MathUtil::remapBounds(plane->gunHP, 100, 0, 0, 0.2);
            plane->gunHP = 0;
            if (plane->hp < 0) plane->hp = 0;
        }

        // Only play sound every 7th shot to avoid overlap
        if (shotCounter++ % 7 == 0 || shotCounter == 0)
        {

            if (shotCounter == 0)
            {
                ShootSoundEngine->play2D(ASSETS "audio/shoot.wav", false);
                return;
            }

            if (MathUtil::randBool())
            {
            ShootSoundEngine->play2D(ASSETS "audio/shoot_high.wav", false);
	            
            } else
            {

                if (MathUtil::randBool())
                {
					ShootSoundEngine->play2D(ASSETS "audio/shoot_low.wav", false);
                } else
                {
					ShootSoundEngine->play2D(ASSETS "audio/shoot.wav", false);
                }
	            
            }
        }
    }
    // no shooting
    else
    {
        // disable PostProcessing Shake effect
        ApplicationGUI::AppGUI->ppBuffer->shake(false);
        ApplicationGUI::AppGUI->ppBuffer->postProcessingActive(false);
        plane->stopShooting();

        plane->gunHP++; // increase gun hp
        if (plane->gunHP > 100) plane->gunHP = 100;
        shotCounter = 0;
    }

    // Mousesteering
    if (ApplicationSettings::MOUSE_CONTROLS)
    {
        MouseLogger logger = MouseLogger::instance();
        float normX = ((float)logger.x() / ApplicationSettings::WIDTH * 2) - 1;
        float normY = ((float)logger.y() / ApplicationSettings::HEIGHT * 2) - 1;
        float up = -normY / 2;
        float leftTilt, rightTilt;
        leftTilt = 0;
        rightTilt = 0;

        // Rolle Rechts
        if (normX > 0) { // right quad
            leftTilt = -abs(normY) * normX; 
            rightTilt = abs(normY) * normX;
        }
        // Rolle links
        if (normX < 0) { // left quad
            leftTilt = -abs(normY) * normX; 
            rightTilt = abs(normY) * normX; 
        }

        plane->tiltLeftWingflaps(delta *(up + leftTilt) / 2);
        plane->tiltRightWingflaps(delta *(up + rightTilt) / 2);
        // Mouse Controls
        plane->tiltRudder(delta * normX);
    }


    plane->update(delta);

    // camera follows plane
    cam->setTarget(plane->getParts()[0]->transform().translation());
    cam->setPosition(cameraPos.translation());
    cam->zoom(-plane->getSpeed() / 500); // FOV to speed adjust

    ApplicationGUI::AppGUI->ppBuffer->hp(plane->hp);
}
