#version 450

in vec4 position;
in vec4 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(std430, binding = 0) buffer model_instances
{
    mat4 models[];
};

out vec4 vert_color;

void main()
{
    if(models.length() == 0) // not instanced
    {
        gl_Position = projection*view*model*position;
    }
    else // instanced
    {
        gl_Position = projection * view * model * models[gl_InstanceID] * position;
    }
    vert_color = color;
}