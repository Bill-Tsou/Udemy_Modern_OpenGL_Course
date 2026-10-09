#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB   // make GLFW include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <cstdio>

int main() {
    glfwInit();
    // must set in macOS, otherwise, only retrieve 2.1 legacy context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* win = glfwCreateWindow(800, 600, "M1 OpenGL Window", nullptr, nullptr);
    glfwMakeContextCurrent(win);
    printf("%s\n", glGetString(GL_VERSION));

    while (!glfwWindowShouldClose(win)) {
        int w, h;
        glfwGetFramebufferSize(win, &w, &h);  // Retina：用 framebuffer 尺寸
        glViewport(0, 0, w, h);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
}