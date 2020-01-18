#version 450 core

// Output
out vec4 color;

// Input from vertex shader
in vec4 P;
in vec3 N;

// Matrices
layout(location = 3) uniform mat4 view;

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
layout(location = 5) uniform vec4 light_position;


void main(void)
{
    vec3 n = normalize(N);
    vec3 v = normalize(vec3(inverse(view)*vec4(0.0,0.0,0.0,1.0) - P));
    vec3 l = normalize(light_position.xyz);
    vec3 ambient = (material.ambient*light.ambient).rgb;
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
    color = vec4((ambient+diffuse+specular), 1.0);
}