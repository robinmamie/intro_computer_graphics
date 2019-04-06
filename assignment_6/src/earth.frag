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

uniform sampler2D day_texture;
uniform sampler2D night_texture;
uniform sampler2D cloud_texture;
uniform sampler2D gloss_texture;
uniform bool greyscale;

const float shininess = 20.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);

void main()
{
    /** \todo
    * - Copy your working code from the fragment shader of your Phong shader use it as
    * starting point
    * - instead of using a single texture, use the four texures `day_texure`, `night_texure`,
    * `cloud_texure` and `gloss_texture` and mix them for enhanced effects
    * Hints:
    * - cloud and gloss textures are just greyscales. So you'll just need one color-
    * component.
    * - The texture(texture, 2d_position) returns a 4-vector (rgba). You can use
    * `texture(...).r` to get just the red component or `texture(...).rgb` to get a vec3 color
    * value
    * - use mix(vec3 a,vec3 b, s) = a*(1-s) + b*s for linear interpolation of two colors
     */
    
    vec3 color    = vec3(0.0,0.0,0.0);
    vec3 c_day    = vec3(0.0,0.0,0.0);
    vec3 c_cloud  = vec3(0.0,0.0,0.0);
    vec3 c_night  = vec3(0.0,0.0,0.0);
    
    float gloss   = texture(gloss_texture, v2f_texcoord).r;
    float cloud_f = texture(cloud_texture, v2f_texcoord).r;
    vec3  day     = texture(day_texture, v2f_texcoord).rgb;
    vec3  cloud   = texture(cloud_texture, v2f_texcoord).rgb;
    vec3  night   = texture(night_texture, v2f_texcoord).rgb;

    // Ambient
    c_day   += 0.2 * sunlight * day;
    c_cloud += 0.2 * sunlight * cloud;

    float dot_nl = dot(v2f_normal, v2f_light);

    if (dot_nl > 0.0) {
        // Diffuse
        c_day   += dot_nl * sunlight * day; 
        c_cloud += dot_nl * sunlight * cloud;
        
        vec3 r = 2.0 * v2f_normal * dot_nl - v2f_light;
        float dot_rv = dot(r, v2f_view);
        
        if (dot_rv > 0.0 && gloss == 1.0) {
            // Specular
            c_day += vec3(1.0,1.0,1.0) * pow(dot_rv, shininess) * (1 - cloud_f);
        }
    }
    
    c_day   = mix(c_day, c_cloud, cloud_f);
    c_night = night * (1 - cloud_f);
    
    float day_night_f = (dot_nl + 0.5);
    day_night_f = day_night_f > 1.0 ? 1.0 : (day_night_f < 0.0 ? 0.0 : day_night_f);

    color = mix(c_night, c_day, day_night_f);

    // convert RGB color to YUV color and use only the luminance
    if (greyscale) color = vec3(0.299*color.r+0.587*color.g+0.114*color.b);

    // add required alpha value
    f_color = vec4(color, 1.0);

}
