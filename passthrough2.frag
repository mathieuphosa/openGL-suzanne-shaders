#version 430

in vec4 color; // ignored
in vec2 texcoord; // the interpolated UV coordinates
in vec3 normal;
in vec3 posLight;
in vec3 position;
in vec3 viewPos;
in float ka;
in float kd;
in float ks;

uniform sampler2D tex; // the currently bound texture
out vec4 fColor; // final fragment color
void main()
{
vec3 colorLight = vec3(2.0,0.0,0.0);
vec3 colorDiffuse = vec3(0.0,2.0,0.0);
vec3 norm = normalize(normal);
vec3 lightDir = normalize(posLight - position); 
vec3 viewDir = normalize(viewPos - position);
vec3 reflectDir = reflect(-lightDir, norm);  
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = ks * spec * vec3(0.0,0.0,2.0); 
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * colorDiffuse;
fColor = vec4(ka*colorLight+kd*diffuse+specular, 1.0) * texture(tex, texcoord);
}