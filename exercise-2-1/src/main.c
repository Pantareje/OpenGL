#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_VAOS 1

static GLuint renderingProgram;
static GLuint vao[NUM_VAOS];

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
		"{ color = vec4(0.0, 0.0, 1.0, 1.0); }";

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vShaderSource, NULL);
	glShaderSource(fShader, 1, &fShaderSource, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);

	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);

	return program;
}

static void init(GLFWwindow *window)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glGenVertexArrays(NUM_VAOS, vao);
	glBindVertexArray(vao[0]);

	renderingProgram = createShaderProgram();
}

static void display(GLFWwindow *window, double currentTime)
{
	glPointSize(50.0f + 25.0f * (float)cosf(currentTime));

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);
	glDrawArrays(GL_POINTS, 0, 1);
}

int main()
{
	int result = EXIT_SUCCESS;

	if (!glfwInit()) {
		result = EXIT_FAILURE;
		goto exit;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(600, 600, "Capítulo 2 - Ejercicio 1", NULL, NULL);
	if (!window) {
		result = EXIT_FAILURE;
		goto exit_glfw;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		result = EXIT_FAILURE;
		goto exit_window;
	}

	init(window);

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
