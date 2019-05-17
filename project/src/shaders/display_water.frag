#version 140

in vec2 v2f_tex_coords;
out vec4 f_color;

float perlin_water_3d(vec3 point); // Implemented in noise.frag

void main() {
    // 3rd coordinate would be the time element, given uniformally.
	float noise_val = perlin_water_3d(vec3(v2f_tex_coords, 0.0f));

	f_color = vec4(noise_val);
}
