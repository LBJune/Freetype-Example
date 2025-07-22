#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <windows.h>

#include "font_renderer.h"

int gWidth = 1280, gHeight = 720;

std::string getExeDirectory() {
	char path[MAX_PATH];
	GetModuleFileNameA(nullptr, path, MAX_PATH);
	std::string exePath = path;
	return exePath.substr(0, exePath.find_last_of("\\/"));
}

int main()
{
	//Initialize GLFW library
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	HDC hdc = GetDC(NULL);
	int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
	int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
	ReleaseDC(NULL, hdc);

	gWidth = gWidth * dpiX / USER_DEFAULT_SCREEN_DPI;
	gHeight = gHeight * dpiY / USER_DEFAULT_SCREEN_DPI;

	// Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow(gWidth, gHeight, "FreeType-Example Window", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// GLAD:: Load opengl fuction
	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0) {
		printf("Failed to initialize OpenGL context!\n");
		return -1;
	}

	glViewport(0, 0, gWidth, gHeight);

	FontRenderer fontrender(getExeDirectory() + "\\resources\\fonts\\Source Han Sans CN Regular.otf", 18, FT_RENDER_MODE_NORMAL, dpiX, dpiY);
	FontRenderer fontrenderSize1(getExeDirectory() + "\\resources\\fonts\\Source Han Sans CN Regular.otf", 10, FT_RENDER_MODE_NORMAL, dpiX, dpiY);
	FontRenderer fontrenderSize2(getExeDirectory() + "\\resources\\fonts\\Source Han Sans CN Regular.otf", 14, FT_RENDER_MODE_NORMAL, dpiX, dpiY);
	FontRenderer fontrenderSize3(getExeDirectory() + "\\resources\\fonts\\Source Han Sans CN Regular.otf", 22, FT_RENDER_MODE_NORMAL, dpiX, dpiY);

	FontRenderer fontrenderTTF1(getExeDirectory() + "\\resources\\fonts\\851tegakizatsu Regular.ttf", 18, FT_RENDER_MODE_NORMAL, dpiX, dpiY);
	FontRenderer fontrenderTTF2(getExeDirectory() + "\\resources\\fonts\\MuyaoPleased Regular.ttf", 18, FT_RENDER_MODE_NORMAL, dpiX, dpiY);
	FontRenderer fontrenderTTF3(getExeDirectory() + "\\resources\\fonts\\zcoolwenyiti Regular.ttf", 18, FT_RENDER_MODE_NORMAL, dpiX, dpiY);
	FontRenderer fontrenderTTF4(getExeDirectory() + "\\resources\\fonts\\zcoolqingkehuangyouti Regular.ttf", 18, FT_RENDER_MODE_NORMAL, dpiX, dpiY);

	FontRenderer fontrenderMode1(getExeDirectory() + "\\resources\\fonts\\Source Han Sans CN Regular.otf", 6, FT_RENDER_MODE_LIGHT, dpiX, dpiY);
	FontRenderer fontrenderMode2(getExeDirectory() + "\\resources\\fonts\\Source Han Sans CN Regular.otf", 6, FT_RENDER_MODE_MONO, dpiX, dpiY);
	FontRenderer fontrenderMode3(getExeDirectory() + "\\resources\\fonts\\Source Han Sans CN Regular.otf", 6, FT_RENDER_MODE_LCD, dpiX, dpiY);
	FontRenderer fontrenderMode4(getExeDirectory() + "\\resources\\fonts\\Source Han Sans CN Regular.otf", 6, FT_RENDER_MODE_SDF, dpiX, dpiY);

	// Main loop
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//============================= different color ================================
		fontrender.setColor(1, 0, 0);
		fontrender.renderText(u8"你好，世界！Hello World! 0123456789", 0.1 * gWidth, 0.9 * gHeight);
		fontrender.setColor(0, 1, 0);
		fontrender.renderText(u8"你好，世界！Hello World! 0123456789", 0.1 * gWidth, 0.8 * gHeight);
		fontrender.setColor(0, 0, 1);
		fontrender.renderText(u8"你好，世界！Hello World! 0123456789", 0.1 * gWidth, 0.7 * gHeight);
		fontrender.setColor(1, 1, 1);
		fontrender.renderText(u8"你好，世界！Hello World! 0123456789", 0.1 * gWidth, 0.6 * gHeight);

		//============================= different size ================================
		fontrenderSize1.renderText(u8"你好，世界！Hello World! 0123456789", 0.5 * gWidth, 0.9 * gHeight);
		fontrenderSize2.renderText(u8"你好，世界！Hello World! 0123456789", 0.5 * gWidth, 0.8 * gHeight);
		fontrender.renderText(u8"你好，世界！Hello World! 0123456789",      0.5 * gWidth, 0.7 * gHeight);
		fontrenderSize3.renderText(u8"你好，世界！Hello World! 0123456789", 0.5 * gWidth, 0.6 * gHeight);

		//============================= different ttf ================================
		fontrenderTTF1.renderText(u8"你好，世界！Hello World! 0123456789", 0.1 * gWidth, 0.4 * gHeight);
		fontrenderTTF2.renderText(u8"你好，世界！Hello World! 0123456789", 0.1 * gWidth, 0.3 * gHeight);
		fontrenderTTF3.renderText(u8"你好，世界！Hello World! 0123456789", 0.1 * gWidth, 0.2 * gHeight);
		fontrenderTTF4.renderText(u8"你好，世界！Hello World! 0123456789", 0.1 * gWidth, 0.1 * gHeight);

		//============================= different render mode ================================
		fontrenderMode1.renderText(u8"LIGHT:你好，世界！Hello World! 0123456789", 0.5 * gWidth, 0.4 * gHeight);
		fontrenderMode2.renderText(u8"MONO:你好，世界！Hello World! 0123456789", 0.5 * gWidth, 0.3 * gHeight);
		fontrenderMode3.renderText(u8"LCD:你好，世界！Hello World! 0123456789", 0.5 * gWidth, 0.2 * gHeight);
		fontrenderMode4.renderText(u8"SDF:你好，世界！Hello World! 0123456789", 0.5 * gWidth, 0.1 * gHeight);


		// Render here (swap front and back buffers)
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	//Destroy window and resources
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}