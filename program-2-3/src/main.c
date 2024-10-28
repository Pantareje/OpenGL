#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_VAOS 1

static GLuint renderingProgram;
static GLuint vao[NUM_VAOS];

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
	const char *vShaderSource =
		"#version 430 core\n"
		"void main()\n"
		"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";	
	
	const char *fShaderSource =
		"#version 430 core\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"if (gl_FragCoord.x < 300) color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"else color = vec4(0.0, 0.0, 1.0, 0.0);\n"
		"}";

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vShaderSource, NULL);
	glShaderSource(fShader, 1, &fShaderSource, NULL);
	
	int compiled;

	glCompileShader(vShader);
	checkOpenGlError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		fprintf(stderr, "Error al compilar el sombreador de vértices.\n");
		printShaderLog(vShader);
	}

	glCompileShader(fShader);
	checkOpenGlError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		fprintf(stderr, "Error al compilar el sombreador de fragmentos.\n");
		printShaderLog(fShader);
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	int linked;

	glLinkProgram(program);
	checkOpenGlError();
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		fprintf(stderr, "Error al enlazar el programa.\n");
		printProgramLog(program);
	}

	return program;
}

static bool init(GLFWwindow *window)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glPointSize(30.0f);

	glGenVertexArrays(NUM_VAOS, vao);
	glBindVertexArray(vao[0]);

	renderingProgram = createShaderProgram();
	return renderingProgram != 0;
}

static void display(GLFWwindow *window, double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);
	glDrawArrays(GL_POINTS, 0, 1);
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

	GLFWwindow *window = glfwCreateWindow(600, 600, "Capítulo 2 - Programa 3", NULL, NULL);
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
