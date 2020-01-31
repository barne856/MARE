#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texcoords;
layout(location = 3) uniform mat4 projection;
layout(location = 4) uniform mat4 view;
layout(location = 5) uniform mat4 model;
layout(location = 6) uniform mat3 normal_matrix;
layout(location = 7) uniform mat4 shadow_matrix;

layout(std430, binding = 0) buffer ssbo
{
    mat4 models[];
};

out vec4 P;
out vec3 N;
out vec2 vs_tex_coord;

out vec4 shadow_coord;

void main()
{
    vs_tex_coord = texcoords.xy;
    if(models.length() == 0) // not instanced
    {
        N = normalize(normal_matrix * normal);
        P = view * model * position;
        shadow_coord = shadow_matrix * model * position;
        gl_Position = projection * P;
    }
    else // instanced
    {
        N = normalize(mat3(transpose(inverse(model * models[gl_InstanceID]))) * normal);
        P = view * model * models[gl_InstanceID] * position;
        shadow_coord = shadow_matrix * model * models[gl_InstanceID] * position;
        gl_Position = projection * view * P;
    }
}