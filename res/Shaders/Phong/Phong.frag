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
layout(location = 8) uniform vec3 light_position;

// Texture sampler
layout(binding = 0) uniform sampler2D tex;

uniform float constant_attenuation = 1;
uniform float linear_attenuation = 1;
uniform float quadratic_attenuation = 1;
float strength = 10.0;

layout(binding = 1) uniform sampler2DShadow depth_texture;
in vec4 shadow_coord;

void main(void)
{
    vec3 n = normalize(N);
    vec3 v = normalize(vec3(inverse(view)*vec4(0.0,0.0,0.0,1.0) - P));
    vec3 l = light_position - vec3(P);
    float light_distance = length(l);
    l = l/light_distance;
    float attenuation = 1.0 / (constant_attenuation + linear_attenuation*light_distance + quadratic_attenuation*light_distance*light_distance);
    vec3 eye_direction = vec3(inverse(view)*P);
    vec3 half_vector = normalize(l + eye_direction );
    
    //vec3 ambient = (material.ambient*light.ambient).rgb;
    float a = dot(n, l) * 0.5 + 0.5;
    vec3 ambient = texture(tex, vs_tex_coord).rgb/2.0 + mix(vec3(0.1, 0.1, 0.1), vec3(0.75, 0.75, 0.75), a);
    //vec3 ambient = texture(tex, vs_tex_coord).rgb/4.0;
    float diffuse = max(0.0, dot(n, l));
    float specular = max(0.0, dot(n, half_vector));
    if(diffuse == 0.0)
    {
        specular = 0.0;
    }
    else
    {
        specular = pow(specular, material.shininess) * strength;
    }
    float f = textureProj(depth_texture, shadow_coord);
    vec3 scattered_light = max(f, 0.5)*ambient + f*vec3(light.ambient) * diffuse * attenuation;
    vec3 reflected_light = vec3(light.ambient) * specular * attenuation;
    //vec3 rgb = min(ambient * scattered_light + reflected_light, vec3(1.0));
    vec3 rgb = min(ambient * scattered_light, vec3(1.0));
    color = vec4(rgb, 1.0);
}