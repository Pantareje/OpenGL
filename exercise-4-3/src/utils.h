#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

char* readShaderSource(const char *filePath);

GLuint createVFShaderProgram(const char* vshFilePath, const char* fshFilePath);

bool checkOpenGlError();

void printShaderLog(GLuint shader);
void printProgramLog(GLuint program);
