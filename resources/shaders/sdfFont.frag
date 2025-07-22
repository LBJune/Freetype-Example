#version 330 core
in vec2 texCoordinate;
out vec4 FragColor;

uniform sampler2D textTexture;
uniform vec3 textColor;

const float edgeSmoothness = 0.05;  // 0.01 ~ 0.2

void main()
{    
    float distance = texture(textTexture, texCoordinate).r;
    float signedDistance = (distance - 0.5) * 2.0;
    float alpha = smoothstep(-edgeSmoothness, edgeSmoothness, signedDistance);
    
    FragColor = vec4(textColor, alpha);
}  