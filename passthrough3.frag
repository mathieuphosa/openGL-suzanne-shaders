#version 430
in vec4 color;
in vec2 texcoord;
uniform sampler2D tex;
out vec4 fColor;
void main()
{
fColor = color * texture(tex, texcoord);
}