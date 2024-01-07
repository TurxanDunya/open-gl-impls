#version 330 core     

layout (location = 0) in vec3 inPosition;

uniform mat3 uMtxTransform;

void main()
{                                  
    gl_Position = vec4(inPosition * uMtxTransform, 1.0f);
}