#include "shader.hpp"
#include "../../core/ew/external/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//Shader Definitions
SHADER_NS::Shader::Shader(const char* vertexPath, const char* fragmentPath, const std::string& name) {
    std::cout << "INITIALIZING_SHADER::" << name << std::endl;
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    else {
        std::cout << "SHADER::VERTEX::COMPILATION_SUCCESS\n";
    }

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);  // Use 'fragment' here instead of 'fragmentShader'
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    else {
        std::cout << "SHADER::FRAGMENT::COMPILATION_SUCCESS\n";
    }

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void SHADER_NS::Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void SHADER_NS::Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void SHADER_NS::Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void SHADER_NS::Shader::setMat4(const std::string& name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void SHADER_NS::Shader::setVec3(const std::string& name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void SHADER_NS::Shader::use() {
    glUseProgram(ID);
}

//Texture2D Definitions
SHADER_NS::Texture2D::Texture2D(const char* filePath, int filterMode, int wrapMode, const std::string& name) {
    std::cout << "INITIALIZING_TEXTURE::" << name << std::endl;
    //Texture setup
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    //wrap and filter settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
    //flip the image so its not upside down
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath, &m_width, &m_height, &m_channels, 0);
    //load image
    if (data)
    {
        if (m_channels == 4) { // if alpha channel
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            std::cout << "TEXTURE::USING_RGBA" << std::endl;
        }
        else if (m_channels == 3) { // if no alpha channel
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            std::cout << "TEXTURE::USING_RGB" << std::endl;
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "INITIALIZING_TEXTURE::FAILURE" << std::endl;
    }
    stbi_image_free(data);
}

void SHADER_NS::Texture2D::Bind(unsigned int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

SHADER_NS::Texture2D::~Texture2D() {
    glDeleteTextures(1, &m_id);
}

//Camera Definitions
SHADER_NS::Camera::Camera(GLFWwindow* window) {
    //Point to self
    camera = this;
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}

void SHADER_NS::Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (camera->lockCamera == true) {
        if (camera->firstMouse)
        {
            camera->lastX = xpos;
            camera->lastY = ypos;
            camera->firstMouse = false;
        }

        float xoffset = xpos - camera->lastX;
        float yoffset = camera->lastY - ypos;
        camera->lastX = xpos;
        camera->lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera->yaw += xoffset;
        camera->pitch += yoffset;

        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        direction.y = sin(glm::radians(camera->pitch));
        direction.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        camera->cameraFront = glm::normalize(direction);
    }
}

void SHADER_NS::Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->fov -= (float)yoffset;
    if (camera->fov < 1.0f)
        camera->fov = 1.0f;
    if (camera->fov > 120.0f)
        camera->fov = 120.0f;
}

void SHADER_NS::Camera::processInput(GLFWwindow* window, float dTime) {
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (holdingMenu == false) {
            lockCamera = !lockCamera;
            holdingMenu = true;
            if (lockCamera == true) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    }
    else {
        holdingMenu = false;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraSpeed = static_cast<float>(10.0 * dTime);
    }
    else {
        cameraSpeed = static_cast<float>(2.5 * dTime);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->cameraPos += cameraSpeed * camera->cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->cameraPos -= cameraSpeed * camera->cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->cameraPos -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->cameraPos += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->cameraPos += camera->cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->cameraPos -= camera->cameraUp * cameraSpeed;
}

//ImGui Definitions
SHADER_NS::ImGUI::ImGUI(GLFWwindow* window, Camera* camera_pointer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    camera = camera_pointer;
    std::cout << "INITIALIZING_IMGUI::SUCCESS" << std::endl;
}

void SHADER_NS::ImGUI::StartImGuiFrame() {
    if (camera->lockCamera == false) {
        //Start Drawing
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        //Make Settings Window
        ImGui::Begin("Lighting Settings");
        ImGui::DragFloat3("Light Position", &lightPosition.x, 0.1f);
        ImGui::ColorEdit3("Light Color", &lightColor.x);
        ImGui::Checkbox("Blinn-Phong Lighting", &blinn);
        ImGui::SliderFloat("AmbientK", &ambient_k, 0.0f, 1.0f);
        ImGui::SliderFloat("DiffuseK", &diffuse_k, 0.0f, 1.0f);
        ImGui::SliderFloat("SpecularK", &specular_k, 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess", &shininess, 2, 1024);

        ImGui::End();

        //Actualy Render it
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
