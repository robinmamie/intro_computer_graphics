#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec4 v_position;
layout (location = 1) in vec3 v_normal;

uniform mat4 modelview_projection_matrix;
uniform mat4 modelview_matrix;

void main()
{
	// transform vertex by modelview and projection matrix
	gl_Position = modelview_projection_matrix * v_position;
}
