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


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 780;
const float ASPECT_RATIO = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

using namespace SHADER_NS;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

const char* vertexShaderPath = "assets/vertex.vert";
const char* fragmentShaderPath = "assets/fragment.frag";

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0;
bool firstMouse = true;

float fov = 60.0f;

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
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //texture character
    Texture2D texture("assets/background.png", GL_NEAREST, GL_REPEAT, "ASSIGNMENT_4");

    // allowing blending (transparency)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // texture coordinates
    glEnableVertexAttribArray(1);
    
    //other
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    // Shader setup (using custom library)
    Shader shader(vertexShaderPath, fragmentShaderPath, "ASSIGNMENT_4");

    shader.use();
    shader.setInt("ourTexture", 0);

    //z-buffer
    glEnable(GL_DEPTH_TEST);
    
    // Focus window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Setup Callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    float currentTime = 0;

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
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
        glActiveTexture(GL_TEXTURE0);
        texture.Bind();
        
        float currentFrame = static_cast<float>(time);
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::vec3 direction;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("view", view);

        // Move Camera
        processInput(window);

        glm::mat4 projection;
        //projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

        //set to the vertex shader
        int viewLoc = glGetUniformLocation(shader.ID, "view");
        int projectionLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 40; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);

            // Position the cubes
            model = glm::translate(model, cubepositions[i]);

            // Calculate the angle for rotation
            float angle = 1.0f * i + currentTime * 10.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            // Scaling effect using sin/cos based on time
            float scaleValue = 0.1f + 1.0f * sin(currentTime + i);
            model = glm::scale(model, glm::vec3(scaleValue, scaleValue, scaleValue));

            // Add some movement using sin/cos
            float moveX = 3.2f * cos(currentTime + i);
            float moveY = 3.2f * sin(currentTime + i);
            model = glm::translate(model, glm::vec3(moveX, moveY, 0.0f));

            // Set the modified model matrix
            shader.setMat4("model", model);

            // Draw the object
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::cout << "Shutting down..." << std::endl;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
         cameraSpeed = static_cast<float>(5.0 * deltaTime);
    }
    else {
         cameraSpeed = static_cast<float>(2.5 * deltaTime);
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 120.0f)
        fov = 120.0f;
}

float random() {
    return (-5) - std::rand() % (5 - (-5) + 1);
}