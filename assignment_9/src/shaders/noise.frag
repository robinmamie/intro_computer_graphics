#version 140
#pragma optionNV unroll all

//=============================================================================
//	Exercise code for "Introduction to Computer Graphics 2018"
//     by
//	Krzysztof Lis @ EPFL
//=============================================================================

float hash_poly(float x) {
	return mod(((x*34.0)+1.0)*x, 289.0);
}

// -- Hash function --
// For 1d case, use vec2(c, 0)
int hash_func(vec2 grid_point) {
	return int(hash_poly(hash_poly(grid_point.x) + grid_point.y));
}

// -- Smooth interpolation polynomial --
// Use mix(a, b, blending_weight_poly(t))
float blending_weight_poly(float t) {
	return t*t*t*(t*(t*6.0 - 15.0)+10.0);
}

// -- Gradient table --
// use gradients[hash % NUM_GRADIENTS] to access the gradient corresponding
// to a hashed grid point location
#define NUM_GRADIENTS 12
const vec2 gradients[NUM_GRADIENTS] = vec2[NUM_GRADIENTS](
	vec2( 1,  1),
	vec2(-1,  1),
	vec2( 1, -1),
	vec2(-1, -1),
	vec2( 1,  0),
	vec2(-1,  0),
	vec2( 1,  0),
	vec2(-1,  0),
	vec2( 0,  1),
	vec2( 0, -1),
	vec2( 0,  1),
	vec2( 0, -1)
);

// Constants for FBM
const float freq_multiplier = 2.17;
const float ampl_multiplier = 0.5;
const int num_octaves = 4;

// ==============================================================
// 1D Perlin noise evaluation and plotting

float perlin_noise_1d(float x) {
	/*
	// Note Gradients in the table are 2d, so in the 1D case we use grad.x
	*/
	/** \todo
	 * Evaluate the 1D Perlin noise function at "x" as described
	 * in the handout. You will determine the two grid points
	 * surrounding x, look up their gradients, evaluate the the
	 * linear functions these gradients describe, and interpolate these
	 * values using the smooth interolation polygnomial blending_weight_poly.
	 * Note: gradients in the gradient lookup table are 2D, 
	 */
	return 0.0f;
}

float perlin_fbm_1d(float x) {
	/** \todo
	 * Implement 1D fractional Brownian motion (fBm) as described in the handout.
	 * You should add together num_octaves octaves of Perlin noise, starting at
	 * octave 0. You also should use the frequency and amplitude multipliers
	 * freq_multiplier and ampl_multiplier defined above to rescale each
	 * successive octave.
	 * Note: the GLSL `for` loop may be useful.
	 */
	return 0.0f;
}

// ----- plotting -----

const vec3 plot_foreground = vec3(0.5, 0.8, 0.5);
const vec3 plot_background = vec3(0.2, 0.2, 0.2);

vec3 plot_value(float func_value, float coord_within_plot) {
	return (func_value < ((coord_within_plot - 0.5)*2.0)) ? plot_foreground : plot_background;
}

vec3 plots(vec2 point) {
	// Press D (or right arrow) to scroll

	if(point.y < 0 || point.y > 1.0) {
		return vec3(255, 0, 0);
	}

	float y_inv = 1-point.y;
	float y_rel = y_inv / 0.2;
	int which_plot = int(floor(y_rel));
	float coord_within_plot = fract(y_rel);

	vec3 result;
	if(which_plot < 4) {
		result = plot_value(
 			perlin_noise_1d(point.x * pow(freq_multiplier, which_plot)),
			coord_within_plot
		);
	} else {
		result = plot_value(
			perlin_fbm_1d(point.x) * 1.5,
			coord_within_plot
		);
	}

	return result;
}

// ==============================================================
// 2D Perlin noise evaluation

float perlin_noise(vec2 point) {
	/** \todo
	* Implement 2D perlin noise as described in the handout.
	* You may find a glsl `for` loop useful here, but it's not necessary.
	**/
	return 0.0f;
}

// ==============================================================
// 2D Fractional Brownian Motion

float perlin_fbm(vec2 point) {
	/** \todo
	 * Implement 2D fBm as described in the handout. Like in the 1D case, you
	 * should use the constants num_octaves, freq_multiplier, and
	 * ampl_multiplier. 
	 */
	return 0.0f;
}

// ==============================================================
// 2D turbulence

float turbulence(vec2 point) {

	/** \todo
	 * Implement the 2D turbulence function as described in the handout.
	 * Again, you should use num_octaves, freq_multiplier, and ampl_multiplier.
	 */
	return 0.0f;
}

// ==============================================================
// Procedural "map" texture

const float terrain_water_level = -0.075;
const vec3 terrain_color_water = vec3(0.29, 0.51, 0.62);
const vec3 terrain_color_grass = vec3(0.43, 0.53, 0.23);
const vec3 terrain_color_mountain = vec3(0.8, 0.7, 0.7);

vec3 tex_map(vec2 point) {
	/** \todo
	 * Implement your map texture evaluation routine as described in the
	 * handout. You will need to use your perlin_fbm routine and the
	 * terrain color constants described above.
	 */
	return vec3(0.0f);
}

// ==============================================================
// Procedural "wood" texture

const vec3 brown_dark 	= vec3(0.48, 0.29, 0.00);
const vec3 brown_light 	= vec3(0.90, 0.82, 0.62);

vec3 tex_wood(vec2 point) {
	/** \todo
	 * Implement your wood texture evaluation routine as described in the
	 * handout. You will need to use your 2d turbulence routine and the
	 * wood color constants described above.
	 */
	return vec3(0.0f);
}


// ==============================================================
// Procedural "marble" texture

const vec3 white 			= vec3(0.95, 0.95, 0.95);

vec3 tex_marble(vec2 point) {
	/** \todo
	 * Implement your marble texture evaluation routine as described in the
	 * handout. You will need to use your 2d turbulence routine and the
	 * marble color constants described above.
	 */
	return vec3(0.0f);
}

