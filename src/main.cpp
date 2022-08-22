#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "Application.h"
#include "freeimage.h"

void PrintOpenGLVersion();

int main () {
    srand(time(NULL));
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
    
    const int WindowWidth = ASPECT_WIDTH;
    const int WindowHeight = ASPECT_HEIGHT;
	glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow (WindowWidth, WindowHeight,
        "Eagle Day", NULL, NULL);

    // Icon - Logo
    GLFWimage logo;
	unsigned char* data = Texture::LoadDataPtr(ASSETS "logo.png");
    logo.pixels = data;
    logo.width = 1080;
    logo.height = 1080;
    glfwSetWindowIcon(window, 1, &logo);
    
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
    
    {
        double lastTime=0;
        Application App(window);
        App.start();
        while (!glfwWindowShouldClose (window)) {
            double now = glfwGetTime();
            double delta = now - lastTime;
            lastTime = now;
            // once per frame
            glfwPollEvents();
            App.update((float)delta);
            App.draw();
            glfwSwapBuffers (window);
        }
        App.end();
    }
    
    glfwTerminate();
    return 0;
}


void PrintOpenGLVersion()
{
    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);
}
