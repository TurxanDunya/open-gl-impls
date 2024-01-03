#pragma once

#include <string>
#include <map>

class Program
{
public:
    Program();
    ~Program();
    void attachShader(const char* fileName, unsigned int shaderType);
    void link();
    void use();
    void addUniform(const std::string& uniformName);
    void setValueToUniform(const std::string& uniformName, float value);

private:
    unsigned int m_ProgramId;
    std::map<std::string, unsigned int> m_UniformVars;

    std::string GetShaderFromFile(const char* fileName);
};
