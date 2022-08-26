#include "Application.h"

#include "ApplicationSettings.h"
#include "CloudShader.h"
#include "GUILoader.h"
#include "PlaneLoader.h"
#include "WaterLoader.h"
#include "PlayerPlaneControls.h"
#include "ModelLoader.h"
#include "TextureShader.h"
#include "MouseLogger.h"
#include "AudioManager.h"
#include "WaterLoaderImpl.h"

PlayerPlaneControls* Application::planeControls = nullptr;
Camera* Application::Cam;
EnemyPlane* Application::enemyPlane = nullptr;
GLFWwindow* Application::pWindow = nullptr;


Application::Application(GLFWwindow* pWin) : ShadowGenerator(1024, 1024), AppGUI(new ApplicationGUI(pWin))
{
    // ----------- START ----------- 
    print("APPLICATION", "initialisation.");

    // ----------- STATIC INSTANCE INIT -----------
    ApplicationSettings::instance();
    ModelLoader::instance().init(&Models, &Cloud_Box, &Ocean);
    pWindow = pWin;

    // ----------- GUI INIT -----------
    AppGUI->setGUIStatus(LOADING_SCREEN_GUI, true);


    // ----------- FINISH ----------- 
    print("APPLICATION", "initialisation finish!");
    printDivider();
}

void Application::start()
{
    glEnable(GL_DEPTH_TEST); // postProcessingEffects depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

void Application::update(float dtime)
{
    // ----------- DELTA TIME -----------
    double deltaTime = glfwGetTime() - last; // delta = 1s/hhz, bei 165 = 0.006
    last = glfwGetTime();

    // ----------- 'Singleton' MouseLogger update ----------- 
    double x,y;
    glfwGetCursorPos(pWindow, &x, &y);
    MouseLogger::instance().update(x, y);

    // ----------- GUI Keyboard Input update ----------- 
    AppGUI->updateInputs((float)deltaTime);
    if (AppGUI->status().loadingScreen ||AppGUI->status().escapeMenu) return;

    // ----------- (GAME) Plane Control handler ----------- 
    if (ModelLoader::pPlayerPlane)
    {
        this->planeControls->update(deltaTime);
        if (APPLICATION_ONLINE_MODE)this->enemyPlane->update(deltaTime);
    }


    // ----------- Model Update ----------- 
    if (ModelLoader::pWaterLoader) ModelLoader::pWaterLoader->updateOcean(Cam, deltaTime);
    CloudShader::TimeTranslation = last;
    if (ModelLoader::instance().PlayerPlaneShadowArea)
    {
        ModelLoader::instance().PlayerPlaneShadowArea->transform(Matrix().translation(
            Vector(ModelLoader::pPlayerPlane->getPosition().X, 1.5, ModelLoader::pPlayerPlane->getPosition().Z  + ModelLoader::pPlayerPlane->getPosition().Y)));
    }

    // Scale > 6 gibt clipping Probleme, evtl Kamera anpassen ( oder andere sizes von anderen Objekten)
    ModelLoader::pSkyBox->transform(Matrix().translation(Vector(ModelLoader::pPlayerPlane->getPosition().X, 0, ModelLoader::pPlayerPlane->getPosition().Z)) * Matrix().scale(6));

    Cam->update();
}

void Application::draw()
{
    // ----------- FRAME START ----------- 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!AppGUI->status().startscreenGUI && !AppGUI->status().loadingScreen) {
        // ----------- SHADOW MAPPPING -----------
        std::list<BaseModel*> shadows;
        shadows.push_back(ModelLoader::instance().PlayerPlaneShadowArea);
        for (auto a : Models)
            shadows.push_back(a);

        // Frisst viel Leistung! Gut entscheiden was ShadowMaps erhalten soll
        ShadowGenerator.generate(shadows);
        ShaderLightMapper::instance().activate();

        // ----------- PostProc. Init & 3D SCENE ----------- 
        AppGUI->ppBuffer->preDraw();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Vor Models drawen fuer damit die ShadowPlane angezeigt wird

        for (std::list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it)
        {
            (*it)->draw(*Cam);
        }

        for (std::list<BaseModel*>::iterator it = Ocean.begin(); it != Ocean.end(); it++)
        {
            (*it)->draw(*Cam);
        }

        ModelLoader::PlayerPlaneShadowArea->draw(*Cam);

        ModelLoader::pPlayerPlane->drawParticles(*Cam);

        for (auto cloud : Cloud_Box)
        {
            cloud->draw(*Cam);
        }


        AppGUI->ppBuffer->postDraw();

        // ----------- PostProc. DRAW ----------- 
        AppGUI->ppBuffer->draw(*Cam);
    }

    // ----------- GUI DRAW ----------- 
    AppGUI->draw();


    // ----------- ERROR HANDLING ----------- 
    GLenum Error = glGetError();
    glErrorHandler(Error);
    assert(Error == 0);

    // ----------- FRAME FINISH ----------- 

}
void Application::end()
{
    for (std::list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it)
        delete* it;
    for (std::list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it)
        delete* it;

    for (std::list<BaseModel*>::iterator it = this->Ocean.begin(); it != this->Ocean.end(); ++it)
        delete* it;
    Ocean.clear();
    Models.clear();
}

void Application::glErrorHandler(GLenum err)
{
    switch (err)
    {
        // opengl 2 errors (8)
    case GL_NO_ERROR:
        //std::cout << "GL_NO_ERROR" << std::endl;
        break;

    case GL_INVALID_ENUM:
        std::cout << "GL_INVALID_ENUM" << std::endl;
        break;
    case GL_INVALID_VALUE:
        std::cout << "GL_INVALID_VALUE" << std::endl;
        break;
    case GL_INVALID_OPERATION:
        std::cout << "GL_INVALID_OPERATION" << std::endl;
        break;
    case GL_STACK_OVERFLOW:
        std::cout << "GL_STACK_OVERFLOW" << std::endl;
        break;
    case GL_STACK_UNDERFLOW:
        std::cout << "GL_STACK_UNDERFLOW" << std::endl;
        break;
    case GL_OUT_OF_MEMORY:
        std::cout << "GL_OUT_OF_MEMORY" << std::endl;
        break;
    case GL_TABLE_TOO_LARGE:
        std::cout << "GL_TABLE_TOO_LARGE" << std::endl;
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
        break;
    }
}
