#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

//wacky c++ macros       //msbc debugger function
#define ASSERT(x) if(!x) __debugbreak();//defined our assert macro

//macro for calling GlClearError and GLlogCall before & after a function x
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
    //converts function x to a string and __FILE__ is the meta for what file and __LINE__ is the meta for what line
    //wrap all opengl functions with this, but im lazy so im not gona do that rn



struct ShaderProgramSource 
{
    std::string VertexSource;
    std::string FragmentSource;
};


static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);  //return code for no error is 0
       
}


static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) 
    {   

        std::cout << "Opengl Error: " << error << " in " << file << " from " << function << " at " << line << std::endl;
        return false;
    }

    return true;
}

 



//reading in the shader file we created
static ShaderProgramSource parseShader(const std::string& filepath) 
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {  
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2]; //string stream for vertex and fragment stream
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        //does not find an invalid string position, so if line.find finds #shader this code runs
        if (line.find("#shader") != std::string::npos) {

            //which type of shader it is
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //pointer to the start of our string source
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);


    //shader error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));//specifying message it has to be on the heap not the stack, so we have to do this
        //alloca puts the memory right on the stack
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile <<"<< 
             (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader" << std::endl;

        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();//returns an unsigned int
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //attatching the shaders to the program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
    float positions[] = {
        -0.5f,-0.5f, //0
        0.5f , -0.5f, //1
        0.5f , 0.5f,  //2
        -0.5f, 0.5f  //3

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

    //Buffer id, 0 is usually for invalid
    unsigned int buffer;
    //creating buffer
    glGenBuffers(1, &buffer);
    //bind buffer to our pointer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //filling buffer with data
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW); //Use Dynamic if you were to edit your positions
  



    //where alot of magic happens  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
   
    ShaderProgramSource source = parseShader("res/shaders/basic.shader");

    //std::cout << "vertex" << std::endl;
    //std::cout << source.VertexSource << std::endl;
    //std::cout << "Fragment" << std::endl;
    //std::cout << source.FragmentSource << std::endl;


    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

    //GLCall(glBindVertexArray(0));
    glUseProgram(shader);


    unsigned int ibo; //index buffer object
    GLCall(glGenBuffers(1, &ibo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(unsigned int), indicies, GL_STATIC_DRAW); 
       
    //the location of our uniform on the gpu
    int location = glGetUniformLocation(shader, "u_Color");
    //ASSERT(location != -1); I think this assert does not work due to some flaw with the debugger
    std::cout << location << std::endl;
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));
    

    float r = 0.0f;
    float increment = 0.05;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        
        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

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

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
   
    return 0;
}