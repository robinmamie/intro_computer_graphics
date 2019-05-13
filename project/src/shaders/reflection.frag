#version 430
#extension GL_ARB_explicit_attrib_location : enable

in vec3  v2f_ec_vertex;
in vec3  v2f_normal;
in float v2f_height;

out vec4 f_color;

uniform sampler2D color_map;
uniform sampler2D depth_map;
uniform mat4 projection_matrix;
uniform vec2 resolution;

const float terrain_water_level = -0.03125 + 1e-6;
const vec4 sky_color = vec4(0.8f, 1.0f, 1.0f, 1.0f);
const vec3 camera_view = vec3(0.0f, 0.0f, -1.0f);


bool is_inside_screen(vec2 pixel)
{
    return 0 <= pixel.x || pixel.x < resolution.x ||
           0 <= pixel.y || pixel.y < resolution.y;
}

vec4 reflection()
{
    vec3 ray       = v2f_ec_vertex;
    vec3 normal    = normalize(v2f_normal) * -sign(dot(v2f_normal, v2f_ec_vertex));
    vec3 reflected = normalize(reflect(ray, normal));

    vec3 step_size = 0.1f * reflected;

    for (int i = 0; i < 1000; ++i) {
        ray += step_size;

        vec4 pixelA = projection_matrix * vec4(ray, 1.0);
        pixelA /= pixelA.w;
        vec2 pixel = vec2(pixelA + 1.0f) / 2.0f;

        float depth = texture(depth_map, pixel).r;

        if (depth >= 1.0f) {
            return sky_color;
        }

        if (-ray.z / 5.0f >= depth) {
            vec4 color = texture(color_map, pixel);
            if (color == vec4(1.0f, 1.0f, 1.0f, 1.0f) || !is_inside_screen(pixel)) {
                return sky_color;
            }
            return color;
        }
    }

    return sky_color;
}


void main()
{
    vec2 position = gl_FragCoord.xy / resolution;
    f_color = v2f_height > terrain_water_level ?
                texture(color_map, position) :
                reflection();
}

