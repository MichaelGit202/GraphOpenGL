#include "IndexBuffer.h"
#include "Renderer.h"


//an index buffer is a buffer on gpu to store the data of the indicies between the vertexes.
// So it would be like, draw from position 1 to 2, to 3 to 4

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count) //initializer list
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
        
    //creating buffer
    GLCall(glGenBuffers(1, &m_RendererID));
    //bind buffer to our pointer
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    //filling buffer with data
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); //Use Dynamic if you were to edit your positions


}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unBind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
