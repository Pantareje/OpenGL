#version 430 core

out vec4 color;

void main()
{
	if (gl_FragCoord.x < 300)
		color = vec4(1.0, 0.0, 0.0, 1.0);
	else
		color = vec4(0.0, 0.0, 1.0, 1.0);
}
