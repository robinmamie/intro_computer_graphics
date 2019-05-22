//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 140

in  vec2 v2f_texcoord;
out vec4 f_color;

float clouds(vec2 point); // Implemented in noise.frag

void main()
{
    vec3 sky_basis_color = vec3(0.5, 0.8, 0.9);
    //float cloud_element = clouds(vec2(1,3));
    //cloud_element = (cloud_element>1.0) 1.0: cloud_element;
    f_color = vec4(sky_basis_color, 1.0);
}
