#version 430
#extension GL_ARB_explicit_attrib_location : enable

in vec3  v2f_ec_vertex;
in vec3  v2f_normal;
in float v2f_height;

out vec4 f_color;

float perlin_fbm_3d(vec3 point);

uniform sampler2D color_map;
uniform sampler2D depth_map;
uniform mat4 projection_matrix;
uniform vec2 resolution;
uniform vec3 light_position;
uniform float sky_radius;

const float terrain_water_level = -0.03125 + 1e-6;
const vec3 sky_color = vec3(0.4f, 0.4f, 0.7f);
const vec3 camera_view = vec3(0.0f, 0.0f, -1.0f);
const float shininess = 8.0f;
const vec3 sky_basis_color = vec3(0.5, 0.8, 0.9);
const vec3 white = vec3(1,1,1);

const float float_one_comp = 1.0f - 1e-6;



bool is_inside_screen(vec2 pixel)
{
    // TODO check if sky is better or not
    // Trick to avoid reflection holes on the side: ignore if the ray
    // exits the screen, it clips to the nearest valid value.
    // This is viable because of the continuity of the terrain, giving it a
    // natural look.
    return //1e-6 < pixel.x && pixel.x < float_one_comp &&
           1e-6 < pixel.y && pixel.y < float_one_comp;
}

vec2 from_ray_to_pixel(vec3 ray)
{
    vec4 pixelA = projection_matrix * vec4(ray, 1.0);
    pixelA /= pixelA.w;
    return vec2(pixelA + 1.0f) / 2.0f;
}

vec3 sky(vec3 reflected, vec3 light)
{
    // Squared distance between ray origin and sphere center
    float squared_dist = dot(v2f_ec_vertex, v2f_ec_vertex);

    //Calculating the coefficients of the quadratic equation
    float a = dot(reflected,reflected); // a = d*d
    float b = 2.0f * dot(reflected, v2f_ec_vertex); // b = 2d(o-C)
    float c = squared_dist - sky_radius * sky_radius; // c = (o-C)^2-R^2

    //Calculate discriminant
    float disc = (b*b)-(4.0f*a*c);
    float sqrt_disc = sqrt(disc);
    float t0 = (-b - sqrt_disc) / (2.0f * a);
    float t1 = (-b + sqrt_disc) / (2.0f * a);

    float t = t0 < 0 ? t1 : t0;
    vec3 position = v2f_ec_vertex + reflected * t;
    float dot_rl = dot(reflected, light);
    return 0.7f * (dot_rl > 0.8 ? vec3(1,1,1) : mix(sky_basis_color, white, perlin_fbm_3d(5*position+500)));
}

vec3 reflection()
{
    vec3 ray       = v2f_ec_vertex;
    vec3 normal    = normalize(v2f_normal) * -sign(dot(v2f_normal, v2f_ec_vertex));
    vec3 reflected = normalize(reflect(ray, normal));
    vec3 light     = normalize(light_position - v2f_ec_vertex);

    vec3 step_size = 0.001f * reflected;


    for (int i = 0; i < 1000; ++i) {
        ray += step_size;

        vec2 pixel = from_ray_to_pixel(ray);

        float depth = texture(depth_map, pixel).r;
        if (!is_inside_screen(pixel) || depth >= 1.0f) {
            return sky(reflected, light);
        }

        if (-ray.z > depth) {
            return mix(vec3(texture(color_map, pixel)), sky(reflected, light), i/1000.0);
        }
    }

    return sky(reflected, light);
}

vec4 water_color()
{
    vec3 color     = reflection();
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
    return vec4(color * factor, 0.7f);
}

void main()
{
    //vec2 position = gl_FragCoord.xy / resolution;
    //vec4 color_terrain = texture(color_map, position);
    f_color = water_color();
    //f_color = height_terrain > v2f_height && height_terrain >= 1.0f ?
    //            vec4(1.0f) :
    //            water_color();
}

