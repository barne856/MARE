#version 450 core

// Output
out vec4 color;

// Input from vertex shader
in vec4 P;
in vec3 N;
in vec2 vs_tex_coord;

// Matrices
layout(location = 4) uniform mat4 view;

// Material properties
layout(binding = 0) uniform material_properties
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} material;

// Light properties
layout(binding = 1) uniform light_properties
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
} light;

// Point Light Position
layout(location = 6) uniform vec4 light_position;

// Texture sampler
layout(binding = 0) uniform sampler2D tex;

void main(void)
{
    vec3 n = normalize(N);
    vec3 v = normalize(vec3(inverse(view)*vec4(0.0,0.0,0.0,1.0) - P));
    vec3 l = normalize(light_position.xyz);
    //vec3 ambient = (material.ambient*light.ambient).rgb;
    vec3 ambient = texture(tex, vs_tex_coord).rgb/4.0;
    vec3 diffuse = (material.diffuse*light.diffuse).rgb * max(0.0,dot(n,l));
    vec3 specular;
    if(dot(n,l) < 0.0)
    {
        specular = vec3(0.0);
    }
    else
    {
        specular = (material.specular*light.specular).rgb * pow( max(0.0, dot(reflect(-l, n), v)), material.shininess );
    }
    color = vec4((ambient+diffuse+specular), texture(tex, vs_tex_coord).a);
}