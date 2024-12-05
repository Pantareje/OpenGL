#include "utils.h"
#include "matrix_stack.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>

#define NUM_VAOS 1
#define NUM_VBOS 3

static GLuint renderingProgram;
static GLuint vao[NUM_VAOS];
static GLuint vbo[NUM_VBOS];

static vec3 cameraPos;

static mat4 pMat;

static struct MatrixStack matrixStack = { .size = 0 };

static void initVertices()
{
	const float cubeVertices[] = {
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

	const float pyramidVertices[] = {
		-1.0f, -1.0f, +1.0f,   +1.0f, -1.0f, +1.0f,   +0.0f, +1.0f, +0.0f,
		+1.0f, -1.0f, +1.0f,   +1.0f, -1.0f, -1.0f,   +0.0f, +1.0f, +0.0f,
		+1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, -1.0f,   +0.0f, +1.0f, +0.0f,
		-1.0f, -1.0f, -1.0f,   -1.0f, -1.0f, +1.0f,   +0.0f, +1.0f, +0.0f,

		-1.0f, -1.0f, -1.0f,   +1.0f, -1.0f, +1.0f,   -1.0f, -1.0f, +1.0f,
		+1.0f, -1.0f, +1.0f,   -1.0f, -1.0f, -1.0f,   +1.0f, -1.0f, -1.0f
	};

	const float tetrahedronVertices[] = {
		+1.0f, +1.0f, +1.0f,   +1.0f, -1.0f, -1.0f,   -1.0f, +1.0f, -1.0f,
		-1.0f, -1.0f, +1.0f,   -1.0f, +1.0f, -1.0f,   +1.0f, -1.0f, -1.0f,
		+1.0f, +1.0f, +1.0f,   -1.0f, +1.0f, -1.0f,   -1.0f, -1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,   +1.0f, -1.0f, -1.0f,   +1.0f, +1.0f, +1.0f,
	};

	glGenVertexArrays(NUM_VAOS, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(NUM_VBOS, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedronVertices), tetrahedronVertices, GL_STATIC_DRAW);
}

static void initProjection(int width, int height)
{
	const float aspect = (float)width / (float)height;
	glm_perspective(PI_DIV_3, aspect, 0.1f, 1000.0f, pMat);
}

static bool init(GLFWwindow *window)
{
	bool success = true;

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	renderingProgram = createVFShaderProgram("basic.vsh", "basic.fsh");
	if (renderingProgram == 0) {
		success = false;
		goto exit;
	}

	glm_vec3_copy((vec3){ 0.0f, 0.0f, 25.0f }, cameraPos);

	initVertices();
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	initProjection(width, height);

exit:
	return success;
}

static void display(GLFWwindow *window, double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderingProgram);

	// Asignamos la matriz de perspectiva.

	glUniformMatrix4fv(2, 1, GL_FALSE, pMat[0]);

	// Añadimos a la pila la matriz de la vista.

	vec3 negCameraPos;
	glm_vec3_negate_to(cameraPos, negCameraPos);

	mat4 vMat = GLM_MAT4_IDENTITY_INIT;
	glm_translate(vMat, negCameraPos);

	ms_push(&matrixStack, vMat);

	float t = (float)currentTime;

	// Añadimos el sol.
	
	mat4 sunMatPos = GLM_MAT4_IDENTITY_INIT;
	mat4 sunMatRot = GLM_MAT4_IDENTITY_INIT;

	glm_translate(sunMatPos, (vec3){ 0.0f, 0.0f, 0.0f });
	glm_rotate(sunMatRot, t, (vec3){ 1.0f, 0.0f, 0.0f });

	ms_add(&matrixStack, sunMatPos);
	ms_add(&matrixStack, sunMatRot);

	glUniformMatrix4fv(1, 1, GL_FALSE, ms_top(&matrixStack)[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glFrontFace(GL_CCW);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	ms_pop(&matrixStack);

	// Añadimos el planeta.

	mat4 planetMatPos = GLM_MAT4_IDENTITY_INIT;
	mat4 planetMatRot = GLM_MAT4_IDENTITY_INIT;

	glm_translate(planetMatPos, (vec3){ 4.0f * sinf(0.9f * t), 0.0f, 4.0 * cosf(0.9f * t) });
	glm_rotate(planetMatRot, t, (vec3){ 0.0f, 1.0f, 0.0f });

	ms_add(&matrixStack, planetMatPos);
	ms_add(&matrixStack, planetMatRot);

	glUniformMatrix4fv(1, 1, GL_FALSE, ms_top(&matrixStack)[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glFrontFace(GL_CW);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	ms_pop(&matrixStack);

	// Añadimos la luna.

	// Al ser el último elemento, sólo necesitamos añadir una matriz.
	mat4 moonMat = GLM_MAT4_IDENTITY_INIT;

	// Es equivalente a «traslación * rotación * escala».
	glm_translate(moonMat, (vec3){ 0.0f, 2.0f * sinf(2.2f * t), 2.0f * cosf(2.2f * t) });
	glm_rotate(moonMat, t, (vec3){ 0.0f, 0.0f, 1.0f });
	glm_scale_uni(moonMat, 0.25f);

	ms_add(&matrixStack, moonMat);

	glUniformMatrix4fv(1, 1, GL_FALSE, ms_top(&matrixStack)[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glFrontFace(GL_CW);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Eliminamos la luna y el planeta de la pila de matrices.
	ms_pop(&matrixStack);
	ms_pop(&matrixStack);

	// Añadimos el segundo planeta.
	mat4 secondPlanetMat = GLM_MAT4_IDENTITY_INIT;

	// Es equivalente a «traslación * rotación * escala».
	glm_translate(secondPlanetMat, (vec3){ 8.0f * sinf(0.7f * t - PI_DIV_3), 0.0f, 8.0f * cosf(0.7f * t - PI_DIV_3) });
	glm_rotate(secondPlanetMat, t, (vec3){ 0.0f, 0.0f, 1.0f });

	ms_add(&matrixStack, secondPlanetMat);

	glUniformMatrix4fv(1, 1, GL_FALSE, ms_top(&matrixStack)[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glFrontFace(GL_CCW);
	glDrawArrays(GL_TRIANGLES, 0, 12);	

	// Limpiamos la pila de matrices.
	ms_clear(&matrixStack);
}

static void reshape(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	initProjection(width, height);
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

	GLFWwindow *window = glfwCreateWindow(600, 600, "Capítulo 4 - Ejercicio 3", NULL, NULL);
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

	glfwSetWindowSizeCallback(window, reshape);

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
