#include "utils.h"
#include "constants.h"

#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_VAOS 1
#define NUM_VBOS 2

static GLuint renderingProgram;
static GLuint vao[NUM_VAOS];
static GLuint vbo[NUM_VBOS];

static vec3 cameraPos;
static vec3 cubePos;

static mat4 pMat, vMat;


static void setupVertices()
{
	// Copiado literalmente del libro. Existen métodos más eficaces para definir un cubo.
	const float vertexPositions[] = {
		-1.0f, +1.0f, -1.0f,   -1.0f, -1.0f, -1.0f,   +1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,   +1.0f, +1.0f, -1.0f,   -1.0f, +1.0f, -1.0f,
		
		+1.0f, -1.0f, -1.0f,   +1.0f, -1.0f, +1.0f,   +1.0f, +1.0f, -1.0f,
		+1.0f, -1.0f, +1.0f,   +1.0f, +1.0f, +1.0f,   +1.0f, +1.0f, -1.0f,

		+1.0f, -1.0f, +1.0f,   -1.0f, -1.0f, +1.0f,   +1.0f, +1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,   -1.0f, +1.0f, +1.0f,   +1.0f, +1.0f, +1.0f,

		-1.0f, -1.0f, +1.0f,   -1.0f, -1.0f, -1.0f,   -1.0f, +1.0f, +1.0f,
		-1.0f, -1.0f, -1.0f,   -1.0f, +1.0f, -1.0f,   -1.0f, +1.0f, +1.0f,

		-1.0f, -1.0f, +1.0f,   +1.0f, -1.0f, +1.0f,   +1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, +1.0f,

		-1.0f, +1.0f, -1.0f,   +1.0f, +1.0f, -1.0f,   +1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,   -1.0f, +1.0f, +1.0f,   -1.0f, +1.0f, -1.0f
	};

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(NUM_VBOS, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

static bool init(GLFWwindow *window)
{
	bool success = true;

	glClearColor(0.0, 0.0, 0.0, 1.0);

	renderingProgram = createVFShaderProgram("basic.vsh", "basic.fsh");
	if (renderingProgram == 0) {
		success = false;
		goto exit;
	}

	glm_vec3_copy((vec3){ 0.0f, 0.0f, 420.0f }, cameraPos);
	glm_vec3_copy((vec3){ 0.0f, 0.0f, 0.0f }, cubePos);

	setupVertices();
	glEnable(GL_DEPTH_TEST);

exit:
	return success;
}

static void display(GLFWwindow *window, double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUseProgram(renderingProgram);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	const float aspect = (float)width / (float)height;

	glm_perspective(PI_DIV_3, aspect, 0.1f, 1000.0f, pMat);

	vec3 negCameraPos;
	glm_vec3_negate_to(cameraPos, negCameraPos);

	glm_mat4_identity(vMat);
	glm_translate(vMat, negCameraPos);

	glUniform1f(1, (float)currentTime);
	glUniformMatrix4fv(2, 1, GL_FALSE, vMat[0]);
	glUniformMatrix4fv(3, 1, GL_FALSE, pMat[0]);

	// Estos datos se almacenan en el VAO, por lo que no sería necesario volver a establecerlos.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100000);
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

	GLFWwindow *window = glfwCreateWindow(600, 600, "Capítulo 4 - Programa 2", NULL, NULL);
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
