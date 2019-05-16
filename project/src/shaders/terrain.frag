//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 140
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_enhanced_layouts : enable

uniform vec3 light_position; // Eye-space light position

in vec3  v2f_ec_vertex;
in vec3  v2f_normal;
in float v2f_height;

// TODO specify location?
layout(location = 0) out vec4 f_color;
layout(location = 1) out vec4 f_depth;
layout(location = 2) out vec4 f_height;

const vec3  sunlight = vec3(1.0, 0.941, 0.898);
// Small perturbation to prevent "z-fighting" on the water on some machines...
const float terrain_water_level    = 0 + 1e-6;
const vec3  terrain_color_water    = vec3(0.29, 0.51, 0.78);
const vec3  terrain_color_mountain = vec3(0.8, 0.5, 0.4);
const vec3  terrain_color_grass    = vec3(0.33, 0.43, 0.18);
const vec3 sky_color = vec3(0.6f, 1.0f, 1.0f);

void main()
{
	float height = v2f_height;

    vec3 material = mix(terrain_color_grass,
                        terrain_color_mountain,
                        (height - terrain_water_level) * 2);
	float shininess = 0.5f;

    // Ambient
    vec3 color = vec3(0.2);

    vec3 N = normalize(v2f_normal) * -sign(dot(v2f_normal, v2f_ec_vertex));
    vec3 light   = normalize(light_position - v2f_ec_vertex);
    float dot_nl = dot(N, light);
    vec3 view    = normalize(-v2f_ec_vertex);
    float dot_vn = dot(view, N);

    if (dot_vn > 0.0 && dot_nl > 0.0) {
        // Diffuse
        color += dot_nl;

        vec3 r = reflect(-light, N);
        float dot_rv = dot(r, view);

        if (dot_rv > 0.0) {
            // Specular
            color += pow(dot_rv, shininess);
        }
    }

    color *= material * sunlight;

	f_color = vec4(color, 1.0f);
    f_depth = vec4(vec3(-v2f_ec_vertex.z), 1.0f);//gl_FragCoord.z), 1.0f);
    f_height = vec4(vec3((v2f_height + 1.0) / 2.0f), 1.0f);
    //f_depth = vec4(vec3(-v2f_ec_vertex.z), 1.0f);//vec4(vec3(length(v2f_ec_vertex)), 1.0f);
}
