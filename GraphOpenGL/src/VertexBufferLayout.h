#pragma once
#include <vector>
#include <GL/glew.h>
#include "Renderer.h"
#include <stdexcept>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type) 
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};



class VertexBufferLayout {

	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;
	public:
		VertexBufferLayout()
			: m_Stride(0) {}
				//initializer list, constructor body


		template<typename T>
		void Push(unsigned int count) {
			std::runtime_error(false);
		}

		template<>
		void Push<float>(unsigned int count)
		{
			m_Elements.push_back({GL_FLOAT, static_cast<unsigned int>(count), GL_FALSE});
			m_Stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count; //4 bytes, (size of )
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, static_cast<unsigned int>(count), GL_FALSE });
			m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count; //4 bytes, (size of )
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, static_cast<unsigned int>(count), GL_TRUE });
			m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count; //4 bytes, (size of )
		}


		inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
		inline unsigned int getStride() const { return m_Stride; }

};