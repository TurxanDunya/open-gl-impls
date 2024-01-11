#version 330 core     

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoord;

uniform mat4 uMtxTransform;

void main()
{
    gl_Position = uMtxTransform * vec4(inPosition, 1.0f);

    texCoord = inTexCoord;
}