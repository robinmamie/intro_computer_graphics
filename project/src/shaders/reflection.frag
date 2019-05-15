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
uniform vec3 light_position;

const float terrain_water_level = -0.03125 + 1e-6;
const vec3 sky_color = vec3(0.6f, 1.0f, 1.0f);
const vec3 camera_view = vec3(0.0f, 0.0f, -1.0f);
const float shininess = 8.0f;


bool is_inside_screen(vec2 pixel)
{
    return 0 <= pixel.x || pixel.x < 1.0f ||
           0 <= pixel.y || pixel.y < 1.0f;
}

vec4 reflection()
{
    vec3 ray       = v2f_ec_vertex;
    vec3 normal    = normalize(v2f_normal) * -sign(dot(v2f_normal, v2f_ec_vertex));
    vec3 reflected = normalize(reflect(normalize(ray), normal));

    vec3 step_size = 0.001f * reflected;


    for (int i = 0; i < 1000; ++i) {
        ray += step_size;

        vec4 pixelA = projection_matrix * vec4(ray, 1.0);
        pixelA /= pixelA.w;
        vec2 pixel = vec2(pixelA + 1.0f) / 2.0f;

        float depth = texture(depth_map, pixel).r;

        if (depth >= 1.0f) {
            return sky_color;
        }

        if (-ray.z > depth) {
            vec3 color = vec3(texture(color_map, pixel));
            if (color == sky_color) {
                continue;
            }
            if (!is_inside_screen(pixel)) {
                return sky_color;
            }
            return color;//vec4(vec3(i/10.0f), 1.0f);//color;
        }
    }

    return sky_color;
}

vec4 water_color()
{
    vec4 color     = reflection();
    vec3 light     = normalize(light_position - v2f_ec_vertex);
    vec3 normal    = normalize(v2f_normal) * -sign(dot(v2f_normal, v2f_ec_vertex));
    float dot_nl   = dot(normal, light);
    vec3 view      = normalize(-v2f_ec_vertex);
    float factor   = 0.2;

    if (dot_nl > 0.0) {
        factor += dot_nl;

        vec3 r = reflect(-light, normal);
        float dot_rv = dot(r, view);

        if (dot_rv > 0.0) {
            factor += pow(dot_rv, shininess);
        }
    }
    return vec4(vec3(color * factor), 1.0f);
}

void main()
{
    vec2 position = gl_FragCoord.xy / resolution;
    f_color = v2f_height > terrain_water_level ?
                texture(color_map, position) :
                water_color();
}

