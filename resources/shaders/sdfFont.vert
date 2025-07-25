#version 330 core
layout (location = 0) in vec4 position; // <vec2 pos, vec2 tex>
out vec2 texCoordinate;

void main()
{
    gl_Position = vec4(position.xy, 0.0, 1.0);
    texCoordinate = position.zw;
}  