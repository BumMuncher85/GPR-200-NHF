#include "../core/Shader/shader.hpp"

#include <ew/external/stb_image.h>
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 780;
const float ASPECT_RATIO = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

using namespace SHADER_NS;


float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

const char* vertexShaderPath = "assets/vertex.vert";
const char* fragmentShaderPath = "assets/fragment.frag";
const char* vertexShaderBGPath = "assets/vertex_bg.vert";
const char* fragmentShaderBGPath = "assets/fragment_bg.frag";

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW failed to initialize!" << std::endl;
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment #2", NULL, NULL);
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
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //texture character
    Texture2D texture("assets/thwomp.png", GL_NEAREST, GL_REPEAT, "ASSIGNMENT_2");
    //texture background
    Texture2D texture_bg("assets/background.png", GL_NEAREST, GL_REPEAT, "ASSIGNMENT_2");
    //texture background icon
    Texture2D texture_bg_icon("assets/full_ribbon.png", GL_NEAREST, GL_REPEAT, "ASSIGNMENT_2");

    // allowing blending (transparency)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // texture coordinates
    glEnableVertexAttribArray(2);
    
    //other
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    // Shader setup (using custom library)
    Shader shader(vertexShaderPath, fragmentShaderPath, "ASSIGNMENT_2");
    Shader shader_bg(vertexShaderBGPath, fragmentShaderBGPath, "ASSIGNMENT_2");

    // ENABLES WIREFRAME (ooga booga
    //glPolygonMode(GL_FRONT, GL_LINE);
    //glPolygonMode(GL_BACK, GL_LINE);

    shader.use();
    shader.setInt("ourTexture", 0);
    shader_bg.use();
    shader_bg.setInt("ourTexture", 0);
    shader_bg.setInt("ourTexture2", 0);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Clear framebuffer
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update uniforms
        float time = glfwGetTime();

        // Render
        glBindVertexArray(VAO);

        shader_bg.use();
        shader_bg.setFloat("time", time);
        shader_bg.setFloat("fragTime", time);
        glActiveTexture(GL_TEXTURE0);
        texture_bg.Bind();
        glActiveTexture(GL_TEXTURE1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        texture_bg_icon.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        shader.use();
        shader.setFloat("time", time);
        glActiveTexture(GL_TEXTURE0);
        texture.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::cout << "Shutting down..." << std::endl;
}