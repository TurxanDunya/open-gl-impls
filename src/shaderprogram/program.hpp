#pragma once

#include <string>

class Program
{
public:
    Program();
    ~Program();
    void attachShader(const char* fileName, unsigned int shaderType);
    void link();
    void use();

private:
    unsigned int m_ProgramId;

    std::string GetShaderFromFile(const char* fileName);
};
