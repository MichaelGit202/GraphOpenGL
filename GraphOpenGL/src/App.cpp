#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"






int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }



    //making glfw wrapper use opengl 3.3 with the core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    glfwSwapInterval(1); //matches up framerate with vsync

    /*Check to see if glew was loaded correctly*/
    if (glewInit() != GLEW_OK)
          std::cout << "GLEW ERROR" << std::endl;

    //std::cout << glGetString(GL_VERSION);
   

    /*Creating a triangle*/
    //Creating the buffer in memory to put out triangle in
    //verticies (literally just positions)
    float positions[] = {  //tri vertex, tri vertex, texture coord, texture coord
        -0.5f,-0.5f, 0.0f, 0.0f,//0
        0.5f , -0.5f, 1.0f, 0.0f, //1
        0.5f , 0.5f,  1.0f, 1.0f,//2
        -0.5f, 0.5f,  0.0f, 1.0f //3

    };

    //this is like the draw order of the verts
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0
    };


    //vertex array buffer
    unsigned int vao;
    GLCall(glGenVertexArrays(1, & vao));
    GLCall(glBindVertexArray(vao));

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>((static_cast<unsigned int>(2)));
    va.AddBuffer(vb, layout);

    unsigned int buffer;
    //where alot of magic happens  


    Shader shader("res/shaders/Basic.shader");  
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
     
    Texture tex("res/textures/ChernoLogo.png");
    tex.Bind();
    shader.SetUniform1i("u_Texture", 0);


   
    // ShaderProgramSource source = parseShader("res/shaders/basic.shader");
    //unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);


    IndexBuffer ib(indicies, 6);
  


    va.unBind();
    vb.unBind();
    ib.unBind();
    shader.Unbind();



    Renderer renderer;
    float r = 0.0f;
    float increment = 0.05;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        

        renderer.Draw(va, ib, shader);
 

        if (r > 1.0f)
            increment = -0.05;
        else if (r < -1.0f)
            increment = 0.05;

        r += increment;

        //Because opengl is a statemachine up where we bound our triangle to the buffer, that state is loaded into the buffer, so opengl will load that
        //glDrawArrays(GL_TRIANGLES, 0, 6);
    
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
      

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        glfwPollEvents();
        
       
    }

    va.unBind();
    shader.Unbind();
    vb.unBind();
    ib.unBind();

    glfwTerminate();
   
    return 0;
}