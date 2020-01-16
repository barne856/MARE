#version 450

layout(location = 0) in vec4 position;
layout(location = 1) uniform mat4 projection;
layout(location = 2) uniform mat4 view;
layout(location = 3) uniform mat4 model;

layout(std430, binding = 0) buffer ssbo
{
    mat4 models[];
};

void main()
{
    if(models.length() == 0)
    {
        gl_Position = projection * view  * model * position;
    }
    else
    {
        gl_Position = projection * view * models[gl_InstanceID] * model * position;
    }
}