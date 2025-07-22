#pragma once
#include <string>
#include <glad/gl.h>

extern int gWidth, gHeight;
extern std::string getExeDirectory();

class ivec2 {
public:
    int x, y;

    ivec2(int x = 0, int y = 0) : x(x), y(y) {}
};

class vec3 {
public:
    float x, y, z;

    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
};

std::u32string utf8_to_utf32(const std::string& str);
std::string loadShaderFromFile(const std::string& filePath);
GLuint compileShader(GLenum type, const std::string& source);
GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
