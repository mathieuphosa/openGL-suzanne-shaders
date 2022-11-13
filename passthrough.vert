#version 430
in vec4 vPosition;
in vec4 vColor;
uniform mat4 MVP;
out vec4 color;
void main()
{
	color = vColor;
	gl_Position = MVP*vPosition;
}