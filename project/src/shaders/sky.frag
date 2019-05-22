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

const vec3 sky_basis_color = vec3(0.5, 0.8, 0.9);
const vec3 white = vec3(1,1,1);
const vec3 sun = vec3(1,1,0.6);

void main()
{
    
    float clouds_coeff = perlin_fbm_3d(5*position+500);

    if (position.z > 0.99f)
        f_color = vec4(mix(sun, white, clouds_coeff), 1);
    else
        f_color = vec4(mix(sky_basis_color, white, clouds_coeff), 1);
}
