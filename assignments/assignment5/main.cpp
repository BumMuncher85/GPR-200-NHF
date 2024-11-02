#include "../core/Shader/shader.hpp"

#include <ew/external/stb_image.h>
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 780;
const float ASPECT_RATIO = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

using namespace SHADER_NS;

//what
Camera* Camera::camera = nullptr;
Camera* ImGUI::camera = nullptr;

//SIX OF THEM - SIX SIDES TO THE SQAURE
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
};

const char* vertexShaderPath = "assets/vertex.vert";
const char* fragmentShaderPath = "assets/fragment.frag";
const char* lightVertexShaderPath = "assets/light.vert";
const char* lightFragmentShaderPath = "assets/light.frag";

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float random();

int main() {
    std::srand(std::time(0)); 

    glm::vec3 cubepositions[40] = {
           glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random()),
            glm::vec3(random(),  random(),  random())
    };

    if (!glfwInit()) {
        std::cerr << "GLFW failed to initialize!" << std::endl;
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment #5", NULL, NULL);
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
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //attributes (index, range, type, if normalize, length, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //location in d shader
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Initialization
    unsigned int lightVBO, lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //attributes (index, range, type, if normalize, length, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //location in d shader

    //texture character
    Texture2D texture("assets/background.png", GL_NEAREST, GL_REPEAT, "ASSIGNMENT_5");

    // allowing blending (transparency)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //other
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Shader setup (using custom library)
    Shader shader(vertexShaderPath, fragmentShaderPath, "ASSIGNMENT_5");
    Shader lightShader(lightVertexShaderPath, lightFragmentShaderPath, "ASSIGNMENT_5");

    shader.use();
    shader.setInt("ourTexture", 0);

    lightShader.use();

    //z-buffer
    glEnable(GL_DEPTH_TEST);

    // Focus window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Setup Camera
    Camera camera(window);

    float currentTime = 0;

    //Setup ImGui
    ImGUI imGui(window, &camera);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Update Time
        currentTime += deltaTime;

        // Clear framebuffer
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //clear z-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update uniforms
        float time = glfwGetTime();

        // Render
        glBindVertexArray(VAO);

        // Finalize Shader
        shader.use();
        shader.setFloat("time", time);
        //light settings
        shader.setVec3("lightPosition", imGui.lightPosition);
        shader.setVec3("lightColor", imGui.lightColor);
        shader.setVec3("viewPosition", camera.cameraPos);
        shader.setFloat("ambient_k", imGui.ambient_k);
        shader.setFloat("diffuse_k", imGui.diffuse_k);
        shader.setFloat("specular_k", imGui.specular_k);
        shader.setFloat("shininess", imGui.shininess);
        shader.setBool("blinn", imGui.blinn);
      
        //bind texture
        glActiveTexture(GL_TEXTURE0);
        texture.Bind();

        float currentFrame = static_cast<float>(time);
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::vec3 direction;

        direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        direction.y = sin(glm::radians(camera.pitch));
        direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));

        glm::mat4 view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
        shader.setMat4("view", view);

        // Move Camera
        camera.processInput(window, deltaTime);

        glm::mat4 projection;
        //projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        projection = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

        //set to the vertex shader
        int viewLoc = glGetUniformLocation(shader.ID, "view");
        int projectionLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        //shouldnt need
        //imGui.lightPosition = camera.cameraPos;
        //extra?
       // glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 40; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, cubepositions[i]);

            float angle = 1.0f * i + currentTime * 10.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            float scaleValue = 0.1f + 1.0f * sin(currentTime + i);
            model = glm::scale(model, glm::vec3(scaleValue, scaleValue, scaleValue));

            float moveX = 3.2f * cos(currentTime + i);
            float moveY = 3.2f * sin(currentTime + i);
            model = glm::translate(model, glm::vec3(moveX, moveY, 0.0f));

            shader.setMat4("model", model);

            // Draw the object
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // Use the light shader
        lightShader.use();
        lightShader.setVec3("color", imGui.lightColor);  // Set the light color
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, imGui.lightPosition);  // Position the light cube
        model = glm::scale(model, glm::vec3(0.2f));          // Scale down the light cube for visibility
        lightShader.setMat4("model", model);

        glBindVertexArray(lightVAO);  // Bind the correct VAO for the light cube
        glDrawArrays(GL_TRIANGLES, 0, 36);  // Render the light cube

        //Render ImguiFrame (AFTER CLEAR) *face palm*
        imGui.StartImGuiFrame();

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::cout << "Shutting down..." << std::endl;
}

float random() {
    return (-5) - std::rand() % (5 - (-5) + 1);
}