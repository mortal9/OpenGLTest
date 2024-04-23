#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


static unsigned int CompileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout<<"MYOUT" << message << std::endl;
        glDeleteShader(id);
        free(message);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    unsigned int vertex_shader_index = CompileShader(vertex_shader, GL_VERTEX_SHADER);
    unsigned int fragment_shader_index = CompileShader(fragment_shader, GL_FRAGMENT_SHADER);
     
    glAttachShader(program, vertex_shader_index);
    glAttachShader(program, fragment_shader_index);

    glLinkProgram(program);

    glValidateProgram(program);

    glDeleteShader(vertex_shader_index);
    glDeleteShader(fragment_shader_index);

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

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit()!= GLEW_OK)
    {
        std::cout << "error" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    float poisiton[] = {
           -0.5f, -0.5f,
            0.0f,  0.5f,
            0.5f, -0.5f
    };
    unsigned int p;
    glGenBuffers(1,&p);
    glBindBuffer(GL_ARRAY_BUFFER, p);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), poisiton, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertex_shader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    std::string fragment_shader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0 , 0.0 , 0.0 , 1.0);\n"
        "}\n";


    unsigned int shader = CreateShader(vertex_shader, fragment_shader);
    glUseProgram(shader);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

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