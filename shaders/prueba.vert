#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normals;

out vec2 tex_coord_out;
out vec3 normals_out;
out vec3 frag_pos_out;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
	frag_pos_out = vec3(model * vec4(pos, 1.0));
	gl_Position = mvp * vec4(pos, 1.0);
	tex_coord_out = tex_coord;
	normals_out = mat3(transpose(inverse(model))) * normals;
}