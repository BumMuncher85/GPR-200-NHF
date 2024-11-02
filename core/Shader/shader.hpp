#pragma once

#include "../ew/external/glad.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace SHADER_NS {
    class Shader {
    public:
        // the program ID
        unsigned int ID;

        // constructor reads and builds the shader
        Shader(const char* vertexPath, const char* fragmentPath, const std::string& name);

        // use/activate the shader
        void use();

        // utility uniform functions
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec3(const std::string& name, glm::vec3 value) const;
        void setMat4(const std::string& name, glm::mat4 value) const;
    };
    class Texture2D {
    public:
        Texture2D(const char* filePath, int filterMode, int wrapMode, const std::string& name);
        ~Texture2D();
        void Bind(unsigned int slot = 0); //Bind to a specific texture unit

    private:
        unsigned int m_id; //GL texture handle
        int m_width, m_height, m_channels;
    };
    class Camera {
    public:
        Camera(GLFWwindow* window);
        void processInput(GLFWwindow* window, float dTime);
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    
        static Camera* camera;

        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        float lastX = 400, lastY = 300;
        float yaw = -90.0f;
        float pitch = 0;
        float fov = 60.0f;
        bool firstMouse = true;
        bool lockCamera = true;
        bool holdingMenu = false;
    };
    class ImGUI {
    public:
        ImGUI(GLFWwindow* window, Camera* camera_pointer);
        void StartImGuiFrame();
        glm::vec3 lightPosition = glm::vec3(-8.0f, -8.0f, -12.0f);
        glm::vec3 lightColor = glm::vec3(1,1,1);
        float ambient_k = 0.0f;
        float diffuse_k = 0.0f;
        float specular_k = 0.0f;
        float shininess = 2.0f;
        bool blinn = true;
        static Camera* camera;
    };
}