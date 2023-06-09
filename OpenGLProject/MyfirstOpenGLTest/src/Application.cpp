#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the GLFW library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "OpenGLWindow", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    /* Initialize the GLEW library */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

     /* Output the version of OpenGL */
    std::cout << glGetString(GL_VERSION) << std::endl;
    {
#pragma region buffer

        float positions[] = {
             -0.5f, -0.5f, 0.0f, 0.0f, // 0
              0.5f, -0.5f, 1.0f, 0.0f, // 1
              0.5f,  0.5f, 1.0f, 1.0f, // 2
             -0.5f,  0.5f, 0.0f, 1.0f  // 3
        };

        /* index buffer */
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray vertexArray;

        /* Get Buffers */
        VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout vertexBufferLayout;
        vertexBufferLayout.Push<float>(2);
        vertexBufferLayout.Push<float>(2);
        vertexArray.Addbuffer(vertexBuffer, vertexBufferLayout);

        /* Get Buffers */
        IndexBuffer indexBuffer(indices, 6 * sizeof(unsigned int));

#pragma endregion

        glm::mat4 projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.4f, 0.3f, 0.6f, 1.0f);
        shader.SetUniformMat4f("u_MVP", projection);

        Texture texture("res/textures/Ryzen.png");
        /* Bind the texture to the slot */
        texture.Bind();
        
        shader.SetUniform1i("u_Texture", 0);

        vertexArray.UnBind();
        shader.UnBind();
        vertexBuffer.UnBind();
        indexBuffer.UnBind();

        float red = 0.9f;
        float green = 0.2f;
        float blue = 0.5;
        float increment = 0.05f;

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            /* Draw the triangle */
            shader.Bind();
            shader.SetUniform4f("u_Color", red, green, blue, 1.0f);

            renderer.Draw(vertexArray, indexBuffer, shader);

            if (red > 1.0f)
            {
                increment = -0.05f;
            }
            else if (red < 0.0f)
            {
                increment = 0.05f;
            }

            red += increment;
            green += increment;
            blue += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}