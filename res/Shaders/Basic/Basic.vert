#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) uniform mat4 projection;
layout(location = 3) uniform mat4 view;
layout(location = 4) uniform mat4 model;
layout(location = 5) uniform mat3 normal_matrix;

layout(std430, binding = 0) buffer ssbo
{
    mat4 models[];
};

void main()
{
    if(models.length() == 0)
    {
        gl_Position = normal_matrix[0][0]*projection * view  * model * position;
    }
    else
    {
        gl_Position = normal_matrix[0][0]*projection * view * model * models[gl_InstanceID] * position;
    }
}