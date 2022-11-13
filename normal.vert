#version 430
in vec4 v_coord;
in vec2 v_texcoord;
in vec3 v_normal;

uniform mat4 MVP;
out vec4 color;
out vec2 texcoord;

void main()
{
	texcoord = v_texcoord;
	color = vec4(abs(v_normal), 1.0);
	gl_Position = MVP*v_coord;
}