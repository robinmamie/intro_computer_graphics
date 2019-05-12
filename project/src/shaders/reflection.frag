#version 140
#extension GL_ARB_explicit_attrib_location : enable

in vec3  v2f_ec_vertex;
in vec3  v2f_normal;
in float v2f_height;

out vec4 f_color;

uniform sampler2D color_map;
uniform sampler2D depth_map;
uniform vec2 resolution;
uniform float focal_length;

const float terrain_water_level = -0.03125 + 1e-6;
const vec4 sky_color = vec4(0.8f, 1.0f, 1.0f, 1.0f);
const vec3 camera_view = vec3(0.0f, 0.0f, -1.0f);

bool is_inside_screen(int x, int y) {
    return 0 <= x && x < resolution.x
        && 0 <= y && y < resolution.y;
}

float intersection(vec3 a, vec3 b, vec3 a_dir, vec3 b_dir)
{
    return (a.x * b_dir.y - a.y * b_dir.x + b.y * b_dir.x - b.x * b_dir.y) /
            (a_dir.y * b_dir.x - a_dir.x * b_dir.y);
}

vec4 reflection(vec2 position)
{
    float width               = resolution.x;
    float height              = resolution.y;
    float aspect              = width / height;

    vec3 ray = normalize(v2f_ec_vertex);
    vec3 normal = normalize(v2f_normal) * -sign(dot(v2f_normal, v2f_ec_vertex));
    vec3 reflected = reflect(normal, ray);


    // Use algorithm to go pixel by pixel UNTIL behind or outside screen.
    vec3 t_del = abs(1.0f / reflected);
    vec3 t_max = t_del * 0.5f;
    vec3 average;

    int pixel_x = int(position.x);
    int pixel_y = int(position.y);
    int step_x  = reflected.x > 0 ? 1 : -1;
    int step_y  = reflected.y > 0 ? 1 : -1;
    float depth_travelled;
    float ray_depth;
    float depth;
    float t1 = min(t_max.x, t_max.y);
    float t2;

    do {
        if (t_max.x < t_max.y) {
            pixel_x += step_x;
            t_max.x += t_del.x;
        } else {
            pixel_y += step_y;
            t_max.y += t_del.y;
        }

        t2 = min(t_max.x, t_max.y);
        average = reflected * (t1 + t2) * 0.5f;
        t1 = t2;

        float u = (position.x + 0.5f + average.x) / width - 0.5f;
        float v = (position.y + 0.5f + average.y) / height - 0.5f;
        vec3 cam_ray = vec3(u, v * aspect, -focal_length);

        float depth_travelled = intersection(v2f_ec_vertex, vec3(0.0f), reflected, cam_ray);
        ray_depth = v2f_ec_vertex.z + depth_travelled * reflected.z;

        depth = texture(depth_map, vec2(pixel_x, pixel_y)).r;

    // Not sure for 2nd condition, could be reversed?
    } while (is_inside_screen(pixel_x, pixel_y) && (ray_depth >= depth));


    // If outside -> light blue color for the sky
    if (!is_inside_screen(pixel_x, pixel_y)) {
        return sky_color;
    }

    // Otherwise, give color of pixel hit
    return texture(color_map, vec2(pixel_x, pixel_y));
}


void main()
{
    float height = v2f_height;
    vec2 position = ( gl_FragCoord.xy / resolution.xy );
    f_color = height > terrain_water_level ?
                texture(color_map, position) :
                reflection(position);

}

