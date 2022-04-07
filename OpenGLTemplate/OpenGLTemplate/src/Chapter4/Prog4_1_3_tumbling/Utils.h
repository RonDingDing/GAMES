#ifndef GLEW_STATIC
	#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

class Utils
{
private:
	static std::string readFile(const char* filePath);
	static void printShaderLog(GLuint shader);
	static void printProgramLog(int prog);
	static bool checkOpenGLError();

public:
	Utils();
	static GLuint createShaderProgram(const char* vertFileName, const char* fragFileName);
};