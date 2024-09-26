#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);  //return code for no error is 0

}


bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {

        std::cout << "Opengl Error: " << error << " in " << file << " from " << function << " at " << line << std::endl;
        return false;
    }

    return true;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}



void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const 
{
    shader.Bind();
    va.Bind();
    ib.bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));

}