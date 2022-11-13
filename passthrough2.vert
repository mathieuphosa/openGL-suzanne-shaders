#version 430
in vec4 v_coord;
in vec2 v_texcoord;
in vec3 v_normal;
uniform mat4 MVP;
uniform mat3 NM; // Normal Matrix
uniform vec3 light_position;
uniform float csta;
uniform float cstd;
uniform float csts;
out vec4 color;
out vec2 texcoord;
out vec3 normal;
out vec3 posLight;
out vec3 position;
out vec3 viewPos;
out float ka;
out float kd;
out float ks;

void main()
{
// pass texcoord to fragment shader (not used for the moment)
texcoord = v_texcoord;
// display normals
vec3 N = normalize(NM * v_normal);
color = vec4(abs(N), 1.0f);
gl_Position = MVP*v_coord;
normal = v_normal;
posLight = light_position;
position = vec3(MVP*v_coord);
viewPos = vec3(MVP*vec4(vec3(0.0,0.0,0.0),1.0));
ka = csta;
kd = cstd;
ks = csts;
}