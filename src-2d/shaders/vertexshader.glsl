#version 330 core     

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoord;

uniform mat3 uMtxTransform;

void main()
{
    gl_Position = vec4(uMtxTransform * inPosition, 1.0f);

    texCoord = inTexCoord;
}