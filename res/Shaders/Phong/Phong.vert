#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texcoords;
layout(location = 3) uniform mat4 projection;
layout(location = 4) uniform mat4 view;
layout(location = 5) uniform mat4 model;

layout(std430, binding = 0) buffer ssbo
{
    mat4 models[];
};

out vec4 P;
out vec3 N;
out vec2 vs_tex_coord;

void main()
{
    if(models.length() == 0) // not instanced
    {
        P = model * position;
        N = normalize(mat3(transpose(inverse(model))) * normal);
        gl_Position = projection * view * P;
    }
    else
    {
        P = model * models[gl_InstanceID] * position;
        N = normalize(mat3(transpose(inverse(model * models[gl_InstanceID]))) * normal);
        gl_Position = projection * view * P;
    }
    vs_tex_coord = texcoords.xy;
}