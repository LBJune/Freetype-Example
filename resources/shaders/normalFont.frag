#version 330 core
in vec2 texCoordinate;
out vec4 FragColor;

uniform sampler2D textTexture;
uniform vec3 textColor;

void main()
{    
    FragColor = vec4(textColor, texture(textTexture, texCoordinate).r);
}  