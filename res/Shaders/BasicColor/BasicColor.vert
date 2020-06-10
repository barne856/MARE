#version 450

in vec4 position;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

layout(std430, binding = 0) buffer model_instances
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
        gl_Position = projection * view * model * models[gl_InstanceID] * position;
    }
}