#version 430 core

mat4 buildTranslate(float x, float y, float z)
{
	return mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		  x,   y,   z, 1.0
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

mat4 buildScale(float x, float y, float z)
{
	return mat4(
		  x, 0.0, 0.0, 0.0,
		0.0,   y, 0.0, 0.0,
		0.0, 0.0,   z, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}


layout (location = 0) uniform float offset;

void main()
{
	if (gl_VertexID == 0)
		gl_Position = vec4(+0.25 + offset, -0.25, 0.0, 1.0);
	else if (gl_VertexID == 1)
		gl_Position = vec4(-0.25 + offset, -0.25, 0.0, 1.0);
	else
		gl_Position = vec4(+0.25 + offset, +0.25, 0.0, 1.0);
}
