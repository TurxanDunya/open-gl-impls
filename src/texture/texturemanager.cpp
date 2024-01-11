#include "texturemanager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glad/glad.h>

TextureManager* TextureManager::m_Instance = 0;

TextureManager::TextureManager()
{
}

TextureManager* TextureManager::getInstance()
{
    if (m_Instance)
        return m_Instance;

    m_Instance = new TextureManager();
    return m_Instance;
}

unsigned int TextureManager::loadTexture(std::string fileName)
{
    unsigned int textureId = 0;

    if (m_TextureMap.count(fileName) != 0)
    {
        textureId = m_TextureMap[fileName];
    }
    else
    {
        int width, height, nrChannels;
        unsigned char* imageData = 
            stbi_load((std::string("../images/") + fileName).c_str(), &width, &height, &nrChannels, 0);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(imageData);

        m_TextureMap[fileName] = textureId;
    }

    return textureId;
}

void TextureManager::activateTexture(unsigned int level, unsigned int textureId)
{
    glActiveTexture(level);
    glBindTexture(GL_TEXTURE_2D, textureId);
}