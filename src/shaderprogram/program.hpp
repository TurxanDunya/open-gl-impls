#pragma once

#include <string>
#include <map>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Program
{
public:
    Program();
    ~Program();
    void attachShader(const char* fileName, unsigned int shaderType);
    void link();
    void use();
    void addUniform(const std::string& uniformName);
    void setFloatValueToUniform(const std::string& uniformName, float value);
    void setVec3ValueToUniform(const std::string& uniformName, glm::vec3 vec3);
    void setVec4ValueToUniform(const std::string& uniformName, glm::vec4 vec4);

private:
    unsigned int m_ProgramId;
    std::map<std::string, unsigned int> m_UniformVars;

    std::string GetShaderFromFile(const char* fileName);
};
