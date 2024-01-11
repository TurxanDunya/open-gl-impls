#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform vec4 uColor;
uniform sampler2D imageTexture;

void main()
{
    fragColor = texture(imageTexture, texCoord);
}