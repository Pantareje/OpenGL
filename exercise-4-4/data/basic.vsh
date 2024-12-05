#version 430 core

layout (location = 0) in vec3 position;

layout (location = 1) uniform mat4 m_matrix;
layout (location = 2) uniform mat4 vp_matrix;

out vec4 varyingColor;

void main()
{
	gl_Position = vp_matrix * m_matrix * vec4(position, 1.0);
	varyingColor = vec4(position * 0.5 + 0.5, 1.0);
}
