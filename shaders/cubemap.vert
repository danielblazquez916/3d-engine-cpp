#version 330 core

layout (location = 0) in vec3 pos;

out vec3 tex_coords_out;

uniform mat4 vp;

void main()
{
    tex_coords_out = pos;
    vec4 new_pos = vp * vec4(pos, 1.0);
    gl_Position = new_pos.xyww;
}  