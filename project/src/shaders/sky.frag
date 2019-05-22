//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 140

in  vec3 position;
out vec4 f_color;

float perlin_fbm_3d(vec3 point); // Implemented in noise.frag

void main()
{
    vec3 sky_basis_color = vec3(0.5, 0.8, 0.9);
    vec3 white = vec3(1,1,1);
    f_color = vec4(mix(sky_basis_color, white, perlin_fbm_3d(5*position+500)), 1.0);
}
