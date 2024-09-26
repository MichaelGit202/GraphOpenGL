#include "VertexBuffer.h"
#include "Renderer.h"

//The vertex buffer object stores the verticies 

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //creating buffer
    GLCall(glGenBuffers(1, &m_RendererID));
    //bind buffer to our pointer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    //filling buffer with data
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); //Use Dynamic if you were to edit your positions


}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::unBind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
