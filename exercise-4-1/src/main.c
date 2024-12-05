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
static vec3 tetrahedronPos;

static mat4 pMat, vMat, mMat, mvMat;


static void setupVertices()
{
	// Definimos los vértices y caras de un tetraedro.
	const float vertexPositions[] = {
		+1.0f, +1.0f, +1.0f,   +1.0f, -1.0f, -1.0f,   -1.0f, +1.0f, -1.0f,
		-1.0f, -1.0f, +1.0f,   -1.0f, +1.0f, -1.0f,   +1.0f, -1.0f, -1.0f,
		+1.0f, +1.0f, +1.0f,   -1.0f, +1.0f, -1.0f,   -1.0f, -1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,   +1.0f, -1.0f, -1.0f,   +1.0f, +1.0f, +1.0f,
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

	glm_vec3_copy((vec3){ 0.0f, 0.0f, 30.0f }, cameraPos);
	glm_vec3_copy((vec3){ 0.0f, 0.0f, 0.0f }, tetrahedronPos);

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

	// Una consideración importante sobre convertir el tiempo actual de precisión
	// doble a precisión simple en coma floante es que, transcurrido un tiempo,
	// el error en la precisión se vuelve muy alto y los cálculos fallan. Algunas
	// soluciones usan la variación en el tiempo desed el último fotograma, que puede
	// ser representada sin problema en precisión simple. Sin embargo, aunque para un
	// sólo fotogramasea estable numéricamente, al consistir en operaciones concatenadas
	// (en las que se va perdiendo precisión) el resultado puede diferir del
	// matemáticamente exacto. Como esta demostración es sencilla, simplemente convertimos
	// de precisión doble a precisión simple e ignoramos estas consideraciones.

	for (int i = 0; i < 24; i++) {
		float tf = (float)(currentTime + i);

		mat4 tMat;
		glm_mat4_identity(tMat);
		glm_translate(tMat, (vec3){ 10.0f * sinf(0.35f * tf), 10.0f * cosf(0.52f * tf), 10.0f * sinf(0.70f * tf) });

		mat4 rMat;
		glm_mat4_identity(rMat);
		glm_rotate(rMat, 1.75f * tf, (vec3){ 0.0f, 1.0f, 0.0f });
		glm_rotate(rMat, 1.75f * tf, (vec3){ 1.0f, 0.0f, 0.0f });
		glm_rotate(rMat, 1.75f * tf, (vec3){ 0.0f, 1.0f, 1.0f });

		glm_mat4_mul(tMat, rMat, mMat);

		glm_mat4_mul(vMat, mMat, mvMat);

		glUniformMatrix4fv(1, 1, GL_FALSE, mvMat[0]);
		glUniformMatrix4fv(2, 1, GL_FALSE, pMat[0]);

		// Estos datos se almacenan en el VAO, por lo que no sería necesario volver a establecerlos.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, 12);
	}
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

	GLFWwindow *window = glfwCreateWindow(600, 600, "Capítulo 4 - Ejercicio 1", NULL, NULL);
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
