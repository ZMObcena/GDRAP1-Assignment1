#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <string>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

const float PI = 3.14159265359f;

//Modifier for the model's x position
float x_mod = 0;
//Modifier for the model's y position
float y_mod = 0;
//Modifier for the model's z position
float z_mod = 0;

float x = 0.1f;
float y = 0.1f;
float z = 0.1f;
float scale_x = 4.f;
float scale_y = 4.f;
float scale_z = 1.f;
float theta = 10.f;
float theta_x = 1.f;
float theta_y = 1.f;
float theta_z = 1.f;
float axis_x = 1.f;
float axis_y = 1.f;
float axis_z = 1.f;

bool pressed = false;
void Key_Callback(GLFWwindow* window,
                  int key,
                  int scancode,
                  int action,
                  int mod) 
{
    if (key == GLFW_KEY_D &&
        action == GLFW_REPEAT)
    {
        x += 0.1f;
    }

    if (key == GLFW_KEY_A &&
        action == GLFW_REPEAT)
    {
        x -= 0.1f;
    }

    if (key == GLFW_KEY_W &&
        action == GLFW_REPEAT)
    {
        y += 0.1f;
    }

    if (key == GLFW_KEY_S &&
        action == GLFW_REPEAT)
    {
        y -= 0.1f;
    }

    if (key == GLFW_KEY_Q &&
        action == GLFW_REPEAT)
    {
        scale_x -= 0.1f;
        scale_y -= 0.1f;
        scale_z -= 0.1f;
    }

    if (key == GLFW_KEY_E &&
        action == GLFW_REPEAT)
    {
        scale_x += 0.1f;
        scale_y += 0.1f;
        scale_z += 0.1f;
    }

    if (key == GLFW_KEY_LEFT &&
        action == GLFW_REPEAT)
    {
        theta_y -= 2.f;
    }

    if (key == GLFW_KEY_RIGHT &&
        action == GLFW_REPEAT)
    {
        theta_y += 2.f;
    }

    if (key == GLFW_KEY_UP &&
        action == GLFW_REPEAT)
    {
        theta_x -= 2.f;
    }

    if (key == GLFW_KEY_DOWN &&
        action == GLFW_REPEAT)
    {
        theta_x += 2.f;
    }
}
int main(void)
{
    GLFWwindow* window;

    float fRadius = 1.f;
    float fYOffset = 0.07f;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1200, 1200, "Zion Miguel Obcena", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //Initialize Glad
    gladLoadGL();

    glfwSetKeyCallback(window, Key_Callback);

    //Load the shader file into a string stream
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    //Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();
    //Convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    //Load the shader file into a string stream
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    //Add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();
    //Convert the stream to a character array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    //Create a vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Assign the source to the Vertex Shader
    glShaderSource(vertexShader, 1, &v, NULL);
    //Compile the Vertex Shader
    glCompileShader(vertexShader);

    //Create a fragment shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Assign the source to the fragment Shader
    glShaderSource(fragShader, 1, &f, NULL);
    //Compile the fragment Shader
    glCompileShader(fragShader);

    //Create the shader program
    GLuint shaderProg = glCreateProgram();
    //Attach the compiled vertex shader
    glAttachShader(shaderProg, vertexShader);
    //Attach the compiled fragment shader
    glAttachShader(shaderProg, fragShader);

    //Finalize the compilation process
    glLinkProgram(shaderProg);

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

    glm::mat4 identity_matrix(1.0);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        ////Get the address of variable x in the shader
        //unsigned int xLoc = glGetUniformLocation(shaderProg, "x");
        ////Assign x by using its address
        //glUniform1f(xLoc, x_mod);

        ////Get the address of variable y in the shader
        //unsigned int yLoc = glGetUniformLocation(shaderProg, "y");
        ////Assign x by using its address
        //glUniform1f(yLoc, y_mod);

        ////Get the address of variable z in the shader
        //unsigned int zLoc = glGetUniformLocation(shaderProg, "z");
        ////Assign x by using its address
        //glUniform1f(zLoc, z_mod);

        glm::mat4 transformation_matrix = glm::translate(
            identity_matrix,
            glm::vec3(x, y, z)
        );

        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta_x),
            glm::normalize(glm::vec3(1, 0, 0))
        );


        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta_y),
            glm::normalize(glm::vec3(0, 1, 0))
        );


        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta_z),
            glm::normalize(glm::vec3(0, 0, 1))
        );

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        //currVAO = null
        glBindVertexArray(VAO);
        //currVAO = VAO

        glUseProgram(shaderProg);
        glBindVertexArray(VAO);

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