//=============================================================================
//
//   Exercise code for "Introduction to Computer Graphics"
//     by Julian Panetta, EPFL
//
//=============================================================================
#version 140

// Eye-space fragment position and normals from vertex shader.
in vec3 v2f_normal;
in vec3 v2f_ec_vertex;

uniform vec3 light_position; // Eye-space light position
uniform vec3 light_color;

// Material parameters
uniform vec3  diffuse_color;
uniform vec3 specular_color;
uniform float shininess;

uniform samplerCube shadow_map; // Distances in the shadow map can be accessed with texture(shadow_map, direction).r

out vec4 f_light_contribution;

void main()
{
    // Normalize the interpolated normal
    vec3 N = -sign(dot(v2f_normal, v2f_ec_vertex)) *  // Orient the normal so it always points opposite the camera rays
             normalize(v2f_normal);

    vec3 color = vec3(0.0,0.0,0.0);

    vec3 light = light_position - v2f_ec_vertex;

    vec3 v2f_light = normalize(light);
    float light_distance = length(light);
    float distance = texture(shadow_map, -v2f_light).r;
    float dot_nl = dot(N, v2f_light);

    if (light_distance < distance * 1.01 && dot_nl > 0.0) {
        // Diffuse
        color += dot_nl * diffuse_color;

        vec3 r = -reflect(v2f_light, N); //2.0 * N * dot_nl - v2f_light;
        float dot_rv = dot(r, normalize(-v2f_ec_vertex));

        if (dot_rv > 0.0) {
            // Specular
            color += specular_color * pow(dot_rv, shininess);
        }
    }

    color *= light_color;

    // append the required alpha value
    f_light_contribution = vec4(color, 1.0);
}
