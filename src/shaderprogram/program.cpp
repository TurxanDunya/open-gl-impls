#include "program.hpp"

#include <fstream>

#include "glad/glad.h"

Program::Program()
{
    m_ProgramId = glCreateProgram();
}

Program::~Program()
{
    glDeleteProgram(m_ProgramId);
}

void Program::attachShader(const char* fileName, unsigned int shaderType)
{
    unsigned int shaderId = glCreateShader(shaderType);

    std::string sourceCode = GetShaderFromFile(fileName);
    const char* chSourceCode = &sourceCode[0];

    glShaderSource(shaderId, 1, &chSourceCode, 0);
    glCompileShader(shaderId);
    glAttachShader(m_ProgramId, shaderId);

    glDeleteShader(shaderId);
}

void Program::link()
{
    glLinkProgram(m_ProgramId);
}

void Program::use()
{
    glUseProgram(m_ProgramId);
}

void Program::addUniform(const std::string& uniformName)
{
    m_UniformVars[uniformName] = glGetUniformLocation(m_ProgramId, uniformName.c_str());
}

void Program::setValueToUniform(const std::string& uniformName, float value)
{
    glUniform1f(m_UniformVars[uniformName], value);
}

std::string Program::GetShaderFromFile(const char* fileName)
{
    std::ifstream file(fileName);
    
    std::string data;
    if (file.is_open())
    {
        char readChar;
        while ((readChar = file.get()) != EOF)
        {
            data += readChar;
        }
        file.close();
    }

    return data;
}