#include "../core/Shader/shader.hpp"

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[] = {
    //   X      Y     Z     R     G     B     A
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f
};

const char* vertexShaderPath = "assets/vertex.vert";
const char* fragmentShaderPath = "assets/fragment.frag";

int main() {
    std::cout << "Initializing..." << std::endl;
    if (!glfwInit()) {
        std::cerr << "GLFW failed to initialize!" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
    if (window == NULL) {
        std::cerr << "GLFW failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "GLAD failed to load GL headers" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Initialization
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Shader setup
    SHADER_NS::Shader shader(vertexShaderPath, fragmentShaderPath);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Clear framebuffer
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate shader
        shader.use();

        // Update uniforms
        float time = glfwGetTime();
        shader.setFloat("updateColor", abs(sin(time)));
        shader.setFloat("time", time);


        // Render
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::cout << "Shutting down..." << std::endl;
}
