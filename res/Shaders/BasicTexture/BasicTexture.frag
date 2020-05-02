#version 450

in vec2 vs_tex_coords;

out vec4 color;

uniform sampler2D tex;

void main()
{
    //float depth_value = texture(tex, vs_tex_coords).r;
    //color = vec4(vec3(depth_value), 1.0);
    color = vec4(texture(tex, vs_tex_coords).rgb, 1.0);
}