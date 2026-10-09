#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// VAO will hold multiple VBOs and other types of buffer, to define how a triangle to be drawn
GLuint VAO, VBO, shader;

// === Vertex Shader ===
// note: use OpenGL version 3.3
// gl_Position is a built-in output value
// modify the X and Y position multiply by 0.4
static const char* vShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);   \n\
}                                                               \n\
";

// === Fragment Shader ===
static const char* fShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
out vec4 colour;                                                \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    colour = vec4(1.0, 0.0, 0.0, 1.0);                          \n\
}                                                               \n\
";

void CreateTriangle()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // Create 1 VAO and save the ID to variable VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        // Create 1 VBO and assign the ID to variable VBO
        glGenBuffers(1, &VBO);
        // VBO has multiple targets that can be bound to, in this case, uses GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // to generate buffer data with method
            //  GL_STATIC_DRAW: the allocated vertices are not going to be changed
            //  GL_DYNAMIC_DRAW: the allocated vertices can be changed programmably
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // arguments:
            //  index: location of the attributes we're creating this pointer for (layout value in GLSL)
            //  size: the size of the values that are going to passed in, we have X, Y and Z data
            //  type: type of the data
            //  normalized: whether to normalized the value
            //  stride: the data size that would like to be skipped
            //  offset: the start value of the data
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            // enable the vertex shader attributes in pointer index 0
            glEnableVertexAttribArray(0);

        // unbind array buffer by binding it to 0
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind vertex array by binding it to 0
    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Error creating shader program\n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW initialization failed\n");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile = No backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Bill Demo OpenGL Window", NULL, NULL);
    if (mainWindow == NULL)
    {
        printf("GLFW window creation failed!\n");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // ALlow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialization failed!\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get and handle user input events
        glfwPollEvents();

        // Clear window, set window clear color to Red
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

            glBindVertexArray(VAO);

                glDrawArrays(GL_TRIANGLES, 0, 3);

            glBindVertexArray(0);

        // Un-assign the shader
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
