#include <GLFW/glfw3.h>
#include <cmath>

const float PI = 3.14159265359f;

int main(void)
{
    GLFWwindow* window;

    float fRadius = 1.f;
    float fYOffset = 0.07f;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Zion Miguel Obcena", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_POLYGON);
        for (int i = 0; i < 8; ++i)
        {
            float fAngle = (PI / 4) * i;
            float fNewAngle = fAngle - (PI / 8);

            float x = fRadius * cos(fNewAngle);
            float y = fRadius * sin(fNewAngle) + fYOffset;

            glVertex2f(x, y);
        }
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}