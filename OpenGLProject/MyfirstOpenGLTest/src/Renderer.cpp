#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL_Error] (" << error << ") : " << function <<
            "\n" << file << ":" << file << "\t" << "Line : " << line << std::endl;
        return false;
    }

    return true;
}

void Renderer::Clear() const
{

}

void Renderer::Draw(const VertexArray& vertexArray, IndexBuffer& indexBuffer, Shader& shader) const
{
	/* Draw the triangle */
	shader.Bind();
	vertexArray.Bind();
	indexBuffer.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
