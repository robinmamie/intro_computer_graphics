#version 430
#extension GL_ARB_explicit_attrib_location : enable

out vec4 f_color;

uniform sampler2D color_map;
uniform vec2 resolution;

void main()
{
    vec2 position = gl_FragCoord.xy / resolution;
    f_color = texture(color_map, position);
}

