#version 330
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_color;

out vec4 f_color;


void main()
{
	gl_Position = vec4(in_position, 1);
	f_color = in_color;
}
