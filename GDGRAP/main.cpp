#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

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
    //Initialize Glad
    gladLoadGL();

    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes; // 3D objects
    std::vector<tinyobj::material_t> material; // Textures
    std::string warning, error;

    tinyobj::attrib_t attributes; // Positions, texture data

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    GLfloat vertices[] {
        0.f, 0.5, 0.f,
        -0.5f, 0.f, 0.f,
        0.5f, 0.f, 0.f
    };

    GLuint indices[]{
        0, 1, 2
    };

    //ID of VAO& VBO
    GLuint VAO, VBO, EBO;

    //Initialize VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //currVAO = null
    glBindVertexArray(VAO);
    //currVAO = VAO

    //currVBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //currVBO = VBO

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * attributes.vertices.size(),
        &attributes.vertices[0],
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, // Attrib Index-Index of VBO
        3, // X Y Z
        GL_FLOAT, // Array of GL Floats
        GL_FALSE, // Is Normalized
        3 * sizeof(GLfloat), // Size of component in bytes
        (void*)0 // Stride value
    );

    //curVBO = VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //currVBO = EBO
    //currVAO.VBO.append(EBO)

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    // Enables attrib index 0
    glEnableVertexAttribArray(0);
    
    //currVAO = VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    //currVAO = null

    //currVBO = VBO
    glBindVertexArray(0);
    //currVBO = null

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //currVAO = null
        glBindVertexArray(VAO);
        //currVAO = VAO

        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        //glBegin(GL_POLYGON);
        //for (int i = 0; i < 8; ++i)
        //{
        //    float fAngle = (PI / 4) * i;
        //    float fNewAngle = fAngle - (PI / 8);

        //    float x = fRadius * cos(fNewAngle);
        //    float y = fRadius * sin(fNewAngle) + fYOffset;

        //    glVertex2f(x, y);
        //}
        //glEnd();


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}