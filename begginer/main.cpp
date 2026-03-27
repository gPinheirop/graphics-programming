#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
    if (!glfwInit())
    {
        printf("GLFW deu ruim fio");
        glfwTerminate();
        return 1;
    }
    // setup versions 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // core profile = no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // window
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);

    if (!mainWindow)
    {
        printf("GLFW window creation deu ruim cria");
        glfwTerminate();
        return 1;
    }

    // Get the buffer size information
    int bufferWidht, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidht, &bufferHeight);

    // Set context for glew to use
    glfwMakeContextCurrent(mainWindow);

    // allow modern extensions features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW deu ruim");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // setup viewport size
    glViewport(0, 0, bufferWidht, bufferHeight);

    // Loop until window close

    while (!glfwWindowShouldClose(mainWindow))
    {
        //  get + handle user input
        glfwPollEvents();

        // clear window
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}