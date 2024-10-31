#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_VAOS 1

static GLuint renderingProgram;
static GLuint vao[NUM_VAOS];

static char* readShaderSource(const char *filePath)
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

static void printShaderLog(GLuint shader)
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

static void printProgramLog(GLuint program)
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

static bool checkOpenGlError()
{
	bool foundError = false;
	int glErr;

	while ((glErr = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "Error de OpenGL: %i\n", glErr);
		foundError = true;
	}

	return foundError;
}

static GLuint createShaderProgram()
{
	GLuint program = 0;

	char *vShaderSource = readShaderSource("basic.vsh");
	if (!vShaderSource)
		goto exit;

	char *fShaderSource = readShaderSource("basic.fsh");
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

static bool init(GLFWwindow *window)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glGenVertexArrays(NUM_VAOS, vao);
	glBindVertexArray(vao[0]);

	renderingProgram = createShaderProgram();
	return renderingProgram != 0;
}

static void display(GLFWwindow *window, double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
{
	int result = EXIT_SUCCESS;

	if (!glfwInit()) {
		fprintf(stderr, "Error al iniciar GLFW\n");
		result = EXIT_FAILURE;
		goto exit;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(600, 600, "Capítulo 3 - Ejercicio 1", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Error al iniciar la ventana.\n");
		result = EXIT_FAILURE;
		goto exit_glfw;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		fprintf(stderr, "Error al iniciar las extensiones de OpenGL.\n");
		result = EXIT_FAILURE;
		goto exit_window;
	}

	if (!init(window)) {
		fprintf(stderr, "Error al iniciar el programa.\n");
		result = EXIT_FAILURE;
		goto exit_window;
	}

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


exit_window:
	glfwDestroyWindow(window);

exit_glfw:
	glfwTerminate();

exit:
	return result;
}
