#version 430
in vec4 v_coord;
in vec2 v_textcoord;
in vec3 v_normal;
uniform mat4 MVP;
uniform mat3 NM;
uniform vec3 light_position;
uniform float csta;
uniform float cstd;
uniform float csts;
out vec4 color;
out vec2 texcoord;
void main()
{
texcoord = v_textcoord;
vec3 N = normalize(NM * v_normal);
gl_Position = MVP*v_coord;
vec3 normal = v_normal;
vec3 posLight = light_position;
vec3 position = vec3(MVP*v_coord);
vec3 viewPos = vec3(MVP*vec4(vec3(0.0,0.0,0.0),1.0));
vec3 colorLight = vec3(1.0,0.0,0.0);
vec3 colorDiffuse = vec3(0.0,1.0,0.0);
vec3 norm = normalize(normal);
vec3 lightDir = normalize(posLight - position); 
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * colorDiffuse;
vec3 viewDir = normalize(viewPos - position);
vec3 reflectDir = reflect(-lightDir, norm);  
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = csts * spec * vec3(0.0,0.0,1.0); 
color = vec4(csta*colorLight+cstd*diffuse+specular, 1.0);
}