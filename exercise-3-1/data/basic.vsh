#version 430 core

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
	if (gl_VertexID == 0)
		gl_Position = vec4(+0.25, -0.25, 0.0, 1.0);
	else if (gl_VertexID == 1)
		gl_Position = vec4(-0.25, -0.25, 0.0, 1.0);
	else
		gl_Position = vec4(+0.25, +0.25, 0.0, 1.0);

	// Rotamos el triángulo 2 radianes respecto al eje Z.
	gl_Position = buildRotateZ(2.0) * gl_Position;
}
