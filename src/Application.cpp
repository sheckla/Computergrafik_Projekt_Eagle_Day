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

PlayerPlaneControls* Application::planeControls = nullptr;
AIPlaneController* Application::aiControls = nullptr;
Camera* Application::Cam;
EnemyPlane* Application::enemyPlane = nullptr;
GLFWwindow* Application::pWindow = nullptr;
PostProcessingBuffer* Application::testBuffer = nullptr;


Application::Application(GLFWwindow* pWin) : ShadowGenerator(1024, 1024), AppGUI(new ApplicationGUI(pWin))
{
    aiControls = new AIPlaneController();

    // ----------- START ----------- 
    print("APPLICATION", "initialisation.");
    testBuffer = new PostProcessingBuffer(ApplicationSettings::WIDTH, ApplicationSettings::HEIGHT);

    // ----------- STATIC INSTANCE INIT -----------
    ApplicationSettings::instance();
    ModelLoader::instance().init(&Models, &Cloud_Box, &Ocean);
    Cam = new Camera(pWin);
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

    ApplicationSettings::writeSettings();
    // ----------- DELTA TIME -----------
    double delta = glfwGetTime() - last; // delta = 1s/hhz, bei 165 = 0.006
    last = glfwGetTime();

    // ----------- 'Singleton' MouseLogger update ----------- 
    double x,y;
    glfwGetCursorPos(pWindow, &x, &y);
    MouseLogger::instance().update(x, y);

    // ----------- ppBuffer Elapsed Time update ----------- 
    testBuffer->update(delta);
    if(ApplicationGUI::AppGUI->ppBuffer) ApplicationGUI::AppGUI->ppBuffer->update(delta);

    // ----------- GUI Keyboard/Mouse Input update ----------- 
    AppGUI->updateInputs((float)delta);
    if (AppGUI->status().loadingScreen ||AppGUI->status().escapeMenu|| AppGUI->status().startscreenGUI) return;

    // ----------- Plane Control handler ----------- 
    if (ModelLoader::pPlayerPlane)
    {
        this->planeControls->update(delta);
        if (ApplicationSettings::ONLINE_MODE && ModelLoader::pEnemyPlane)this->enemyPlane->update(delta);
    }

    if (ModelLoader::pAIPlane && ModelLoader::pPlayerPlane)
    {
        //aiControls->update(delta);
    }

    // ----------- Model Update ----------- 
    if (ModelLoader::pWaterLoader) ModelLoader::pWaterLoader->updateOcean(Cam, delta);
    CloudShader::TimeTranslation = last;

    // ShadowPlaneArea adjust
    if (ModelLoader::instance().PlayerPlaneShadowArea)
    {
        ModelLoader::PlayerPlaneShadowArea->transform(Matrix().translation(ModelLoader::pPlayerPlane->getPosition()) * Matrix().translation(0, -10, 0));
    }

    // Scale > 6 gibt clipping Probleme, evtl Kamera anpassen ( oder andere sizes von anderen Objekten)
    ModelLoader::pSkyBox->transform(Matrix().translation(Vector(ModelLoader::pPlayerPlane->getPosition().X, -40, ModelLoader::pPlayerPlane->getPosition().Z)) * Matrix().scale(6));

    Cam->update();
}

void Application::draw()
{
    // ----------- FRAME START ----------- 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!AppGUI->status().startscreenGUI && !AppGUI->status().loadingScreen) {

        // ----------- SHADOW MAPPPING -----------
        // Frisst viel Leistung! Gut entscheiden was ShadowMaps erhalten soll
        std::list<BaseModel*> shadows;

        // ShadowPlane Offset
        Vector prec = ModelLoader::PlayerPlaneShadowArea->transform().translation();
        shadows.push_back(ModelLoader::instance().PlayerPlaneShadowArea);
        for (int i = 0; i < PLANE_PARTS; i++) shadows.push_back(ModelLoader::pPlayerPlane->getParts()[i]);
        ShadowGenerator.generate(shadows);
        ShaderLightMapper::instance().activate();


        // ----------- PostProc. Init & 3D SCENE ----------- 
        AppGUI->ppBuffer->preDraw();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (std::list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it)
        {
            (*it)->draw(*Cam);
        }

        for (std::list<BaseModel*>::iterator it = Ocean.begin(); it != Ocean.end(); it++)
        {
            (*it)->draw(*Cam);
        }


        ModelLoader::pPlayerPlane->drawParticles(*Cam);
        if(ModelLoader::pEnemyPlane != nullptr)
        ModelLoader::pEnemyPlane->drawParticles(*Cam);

        for (auto cloud : Cloud_Box)
        {
            cloud->draw(*Cam);
        }
        ModelLoader::PlayerPlaneShadowArea->draw(*Cam);


        AppGUI->ppBuffer->postDraw();

        // ----------- PostProc. DRAW ----------- 
		testBuffer->preDraw();
        AppGUI->ppBuffer->draw(*Cam);
    }

    // ----------- GUI DRAW -----------
    testBuffer->preDraw();

    AppGUI->draw();
    testBuffer->postDraw();
    testBuffer->draw(*Cam);

    if (ApplicationGUI::AppGUI->status().startscreenGUI || ApplicationGUI::AppGUI->status().escapeMenu) ApplicationGUI::AppGUI->optionsGUI->draw();


    // ----------- ERROR HANDLING ----------- 
    GLenum Error = glGetError();
    glErrorHandler(Error);
    assert(Error == 0);

    // ----------- FRAME FINISH ----------- 

}
void Application::end()
{
    /*for (std::list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it)
       delete (* it);*/
    delete planeControls;
    delete aiControls;
    delete Cam;
    delete enemyPlane;
    delete testBuffer;
    delete AppGUI;

    delete ModelLoader::pWaterLoader;
    //Models.clear();
    exit(0);
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