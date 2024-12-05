#include "utils.h"

#include <stdlib.h>
#include <stdio.h>

char* readShaderSource(const char *filePath)
{
	char *buffer = NULL;

	FILE *file = fopen(filePath, "rb");
	if (!file) {
		fprintf(stderr, "Error al abrir el archivo de sombreador (%s)\n", filePath);
		goto exit;
	}

	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	buffer = malloc(fileSize + 1);
	if (!buffer) {
		fprintf(stderr, "Error al alocar memoria para el sombreador (%s)\n", filePath);
		goto exit_file;
	}

	size_t readSize = fread(buffer, 1, fileSize, file);
	if (readSize != fileSize) {
		fprintf(stderr, "Error leyendo el archivo del sombreador (%s)\n", filePath);
		fprintf(stderr, "Se esperaban %zu bytes, se han leido %zu bytes\n", fileSize, readSize);
		free(buffer);
		buffer = NULL;
		goto exit_file;
	}

	buffer[fileSize] = '\0';

exit_file:
	fclose(file);

exit:
	return buffer;
}


GLuint createVFShaderProgram(const char* vshFilePath, const char* fshFilePath)
{
	GLuint program = 0;

	char *vShaderSource = readShaderSource(vshFilePath);
	if (!vShaderSource)
		goto exit;

	char *fShaderSource = readShaderSource(fshFilePath);
	if (!fShaderSource)
		goto exit;

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	const char *vShaderSources[] = { vShaderSource };
	const char *fShaderSources[] = { fShaderSource };

	glShaderSource(vShader, 1, vShaderSources, NULL);
	glShaderSource(fShader, 1, fShaderSources, NULL);

	free(vShaderSource);
	free(fShaderSource);
	
	int compiled;

	glCompileShader(vShader);
	checkOpenGlError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		fprintf(stderr, "Error al compilar el sombreador de vértices.\n");
		printShaderLog(vShader);
		goto exit;
	}

	glCompileShader(fShader);
	checkOpenGlError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		fprintf(stderr, "Error al compilar el sombreador de fragmentos.\n");
		printShaderLog(fShader);
		goto exit;
	}

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	int linked;

	glLinkProgram(program);
	checkOpenGlError();
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		fprintf(stderr, "Error al enlazar el programa.\n");
		printProgramLog(program);
		program = 0;
		goto exit;
	}

exit:
	return program;
}


bool checkOpenGlError()
{
	bool foundError = false;
	int glErr;

	while ((glErr = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "Error de OpenGL: %i\n", glErr);
		foundError = true;
	}

	return foundError;
}


void printShaderLog(GLuint shader)
{
	int len = 0;
	int written = 0;
	char *log;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

	if (len > 0) {
		log = malloc(len);
		glGetShaderInfoLog(shader, len, &written, log);
		fprintf(stderr, "Error del sombreador:\n%s\n", log);
		free(log);
	}
}

void printProgramLog(GLuint program)
{
	int len = 0;
	int written = 0;
	char *log;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
	
	if (len > 0) {
		log = malloc(len);
		glGetProgramInfoLog(program, len, &written, log);
		fprintf(stderr, "Error del programa:\n%s\n", log);
		free(log);
	}
}

