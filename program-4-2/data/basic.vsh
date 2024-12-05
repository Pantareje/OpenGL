#version 430 core

layout (location = 0) in vec3 position;

layout (location = 1) uniform float tf;
layout (location = 2) uniform mat4 v_matrix;
layout (location = 3) uniform mat4 p_matrix;

out vec4 varyingColor;

mat4 buildTranslate(vec3 a)
{
	return mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		a.x, a.y, a.z, 1.0
	);
}

mat4 buildRotateX(float angle)
{
	float a = cos(angle);
	float b = sin(angle);

	return mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0,  +a,  -b, 0.0,
		0.0,  +b,  +a, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 buildRotateY(float angle)
{
	float a = cos(angle);
	float b = sin(angle);

	return mat4(
		 +a, 0.0,  +b, 0.0,
		0.0, 1.0, 0.0, 0.0,
		 -b, 0.0,  +a, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 buildRotateZ(float angle)
{
	float a = cos(angle);
	float b = sin(angle);

	return mat4(
		 +a,  -b, 0.0, 0.0,
		 +b,  +a, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

void main()
{
	float t = gl_InstanceID + tf;

	vec3 offset = vec3(
		403.0 * sin(203.0 * t / 8000.0),
		401.0 * cos(301.0 * t / 4001.0),
		405.0 * sin(400.0 * t / 6003.0)
	);

	mat4 m_matrix =
		buildTranslate(offset) *
		buildRotateX(0.5 * t) *
		buildRotateY(0.5 * t) *
		buildRotateX(0.5 * t);

	gl_Position = p_matrix * v_matrix * m_matrix * vec4(position, 1.0);
	varyingColor = vec4(position * 0.5 + 0.5, 1.0);
}
