#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"; //forweard include


VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
	//GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {

	Bind();
	vb.bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) 
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArray::Bind() const 
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unBind() const
{
	GLCall(glBindVertexArray(0));
}