#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;
layout(location = 3) uniform mat4 projection;
layout(location = 4) uniform mat4 view;
layout(location = 5) uniform mat4 model;
layout(location = 6) uniform mat3 normal_matrix;

layout(std430, binding = 0) buffer model_instances
{
    mat4 models[];
};

out vec2 vs_tex_coords;

void main()
{
    vs_tex_coords = texcoords.xy;
    if(models.length() == 0)
    {
        gl_Position = normal_matrix[0][0]*projection * view  * model * position;
    }
    else
    {
        gl_Position = normal_matrix[0][0]*projection * view * model * models[gl_InstanceID] * position;
    }
}