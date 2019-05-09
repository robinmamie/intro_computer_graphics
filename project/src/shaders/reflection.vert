#version 400 core

uniform mat4 projection;

layout (location = 0) in vec4 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;

noperspective out vec2 tex_coord;

void main()
{
	tex_coord = v_texcoord;
	gl_Position = projection * v_position;
}
