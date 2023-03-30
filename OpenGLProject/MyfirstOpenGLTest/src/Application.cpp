#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    /* Creates a shader object */
    unsigned int id = glCreateShader(type);

    /* Get source point to point to source code */
    const char* src = source.c_str();

    /* Replaces the source code in a shader object */
    glShaderSource(id, 1, &src, nullptr);

    /* Compiles a shader object */
    glCompileShader(id);
    
    // TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;

        /* Returns a parameter from a shader object */
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(length * sizeof(char));
        
        /* Returns the information log for a shader object */
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")<< " shader !" << std::endl;

        std::cout << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    /* Attaches a shader object to a program object */
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    /* Links a program object */
    glLinkProgram(program);

    /* Validates a program object */
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;

}

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

    /* Initialize the GLEW library */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

     /* Output the version of OpenGL */
    std::cout << glGetString(GL_VERSION) << std::endl;

#pragma region buffer

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    /* Get Buffers */
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    /*Enable or disable a generic vertex attribute array*/
    glEnableVertexAttribArray(0);

    /*define an array of generic vertex attribute data*/
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
#pragma endregion


    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw the triangle */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}