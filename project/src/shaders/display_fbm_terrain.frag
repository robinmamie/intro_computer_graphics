#version 140

in vec2 v2f_tex_coords;
out vec4 f_color;	// Final color output produced by fragment shader.
					// (Can name this anything you want...)

float perlin_fbm(vec2 point); // Implemented in noise.frag

void main() {
	float noise_val = perlin_fbm(v2f_tex_coords+vec2(11.9, 16.7)); //pseudo-seed: 11.9, 17.9

	f_color = vec4(noise_val, noise_val, noise_val, noise_val);
} 
