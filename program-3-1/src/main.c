#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

#define NUM_VAOS 1

static GLuint renderingProgram;
static GLuint vao[NUM_VAOS];

static float x = 0.0f;
static float inc = 0.01f;

static bool init(GLFWwindow *window)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glGenVertexArrays(NUM_VAOS, vao);
	glBindVertexArray(vao[0]);

	renderingProgram = createVFShaderProgram("basic.vsh", "basic.fsh");
	return renderingProgram != 0;
}

static void display(GLFWwindow *window, double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);
	glProgramUniform1f(renderingProgram, 0, x);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	x += inc;
	if (x > 1.0f || x < -1.0f)
		inc = -inc;
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

	GLFWwindow *window = glfwCreateWindow(600, 600, "Capítulo 3 - Programa 1", NULL, NULL);
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
