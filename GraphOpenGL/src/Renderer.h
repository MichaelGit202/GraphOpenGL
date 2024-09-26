#pragma once
#include <GL/glew.h>
#include <string>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "shader.h"


//wacky c++ macros       //msbc debugger function
#define ASSERT(x) if(!x) __debugbreak();//defined our assert macro

//macro for calling GlClearError and GLlogCall before & after a function x
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
    //converts function x to a string and __FILE__ is the meta for what file and __LINE__ is the meta for what line
    //wrap all opengl functions with this, but im lazy so im not gona do that rn


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer 
{
    private:
        
    public:
        void Clear() const;
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};