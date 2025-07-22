#version 330 core
in vec2 texCoordinate;
out vec4 FragColor;

uniform sampler2D textTexture;
uniform vec3 textColor;

void main()
{   
    vec2 pixelSize = 1.0 / vec2(textureSize(textTexture, 0));

    vec3 color = vec3(0.0, 0.0, 0.0);
    vec3 left = texture(textTexture, texCoordinate - vec2(pixelSize.x, 0.0)).rgb;
    vec3 center = texture(textTexture, texCoordinate).rgb;
    vec3 right = texture(textTexture, texCoordinate + vec2(pixelSize.x, 0.0)).rgb;

    color += 0.25 * left;
    color += 1.0 * center;
    color += 0.25 * right;

    float alpha = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b; // or (color.r + color.g + color.b) / 3.0 or  max(color.r, max(color.g, color.b));
    FragColor = vec4(textColor, alpha);
}  