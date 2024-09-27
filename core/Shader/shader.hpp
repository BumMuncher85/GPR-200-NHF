#pragma once

#include "../ew/external/glad.h"
#include <string>

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
}