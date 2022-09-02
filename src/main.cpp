/*
 * MAIN - Programmstart
 *
 * - Window wird generiert
 * - Frameloop
 * - Window icon
 */

#include "Globals.h"
#include "Application.h"
#include "ApplicationSettings.h"
#include "texture.h"

// Forward declaration
void PrintOpenGLVersion();

int main () {
    srand(time(NULL)); // Random-Number init
    FreeImage_Initialise();

    // start GL context and O/S Window using the GLFW helper library
    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
#ifdef __APPLE__
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    ApplicationSettings::instance().readSettings();

    // Window 
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Windowed Mode aspect rate init
	glfwWindowHint(GLFW_SAMPLES, 4);
    ApplicationSettings::WIDTH = mode->width;
    ApplicationSettings::HEIGHT = mode->height;
    GLFWwindow* window;


    if (ApplicationSettings::FULL_SCREEN)
    {
        window = glfwCreateWindow(ApplicationSettings::WIDTH, ApplicationSettings::HEIGHT,
            "Eagle Day - Battle of Britain", monitor, NULL);
    } else
    {
		ApplicationSettings::HEIGHT = mode->height * 0.7;
		ApplicationSettings::WIDTH = mode->width * 0.7;
        window = glfwCreateWindow(ApplicationSettings::WIDTH, ApplicationSettings::HEIGHT,
            "Eagle Day - Battle of Britain", 0, NULL);
    }
    ASPECT_WIDTH = ApplicationSettings::WIDTH;
    ASPECT_HEIGHT = ApplicationSettings::HEIGHT;

    // Window Icon - Logo
    GLFWimage logo;
	unsigned char* data = Texture::LoadDataPtr(ASSETS "img/logo.png");
    logo.pixels = data;
    logo.width = 1080;
    logo.height = 1080;
    glfwSetWindowIcon(window, 1, &logo);

    // creation successfull?
    if (!window) {
        fprintf (stderr, "ERROR: can not open Window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent (window);

#if WIN32
	glewExperimental = GL_TRUE;
	glewInit();
#endif

    PrintOpenGLVersion();
    double lastTime=0;
    Application App(window);
    App.start();
    while (!glfwWindowShouldClose (window)) {
        if (ApplicationSettings::SHUT_DOWN) break;
        double now = glfwGetTime();
        double delta = now - lastTime;
        lastTime = now;
        glfwPollEvents();
        App.update((float)delta);
        App.draw();
        glfwSwapBuffers (window);
    }
    App.end();
    return 0;
}


void PrintOpenGLVersion()
{
    print("Graphics Render Unit:", glGetString(GL_RENDERER));
    print("Supported OpenGL version:", glGetString(GL_VERSION));
    printDivider();
}
