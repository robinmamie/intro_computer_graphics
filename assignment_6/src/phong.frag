//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 140

in vec3 v2f_normal;
in vec2 v2f_texcoord;
in vec3 v2f_light;
in vec3 v2f_view;

out vec4 f_color;

uniform sampler2D tex;
uniform bool greyscale;

const float shininess = 8.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);

void main()
{
    vec3 color = vec3(0.0,0.0,0.0);

    // Ambient
    color += 0.2;

    float dot_nl = dot(v2f_normal, v2f_light);

    if (dot_nl > 0.0) {
        // Diffuse
        color += dot_nl;

        vec3 r = 2.0 * v2f_normal * dot_nl - v2f_light;
        float dot_rv = dot(r, v2f_view);

        if (dot_rv > 0.0) {
            // Specular
            color += pow(dot_rv, shininess);
        }
    }

    // Since the texture is the same everywhere, do the multiplication after
    color *= sunlight * texture(tex, v2f_texcoord).rgb;


    // convert RGB color to YUV color and use only the luminance
    if (greyscale) color = vec3(0.299*color.r+0.587*color.g+0.114*color.b);

    // add required alpha value
    f_color = vec4(color, 1.0);
}
