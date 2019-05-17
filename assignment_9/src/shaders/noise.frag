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
    float c0     = floor(x);
    float phi[2];
    for (int i = 0; i < 2; ++i) {
        float t    = c0 + i;
        float grad = gradients[hash_func(vec2(t, 0.0f)) % NUM_GRADIENTS].x;
        phi[i]     = grad * (x - t);
    }
    return mix(phi[0], phi[1], blending_weight_poly(x - c0));
}

float perlin_fbm_1d(float x) {
    float fbm = 0.0f;
    float am  = 1.0f;
    float fm  = 1.0f;
    for (int i = 0; i < num_octaves; ++i) {
        fbm += am * perlin_noise_1d(x * fm);
        am  *= ampl_multiplier;
        fm  *= freq_multiplier;
    }
	return fbm;
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
#define N_CORNERS 4

float perlin_noise(vec2 point) {
    vec2 c00 = floor(point);
    vec2 p[N_CORNERS] = vec2[N_CORNERS](
                            c00,
                            c00 + vec2(1,0),
                            c00 + vec2(0,1),
                            c00 + vec2(1,1)
                        );
    float phi[N_CORNERS];
    float xs[2];

    for (int i = 0; i < N_CORNERS; ++i) {
        vec2 grad = gradients[hash_func(p[i]) % NUM_GRADIENTS];
        phi[i]    = dot(grad, point - p[i]);
        if (i % 2 != 0) {
            xs[i/2] = mix(phi[i-1], phi[i], blending_weight_poly(point.x - c00.x));
        }
    }
    return mix(xs[0], xs[1], blending_weight_poly(point.y - c00.y));

}

// ==============================================================
// 2D Fractional Brownian Motion

float perlin_fbm(vec2 point) {
    float fbm = 0.0f;
    float am  = 1.0f;
    float fm  = 1.0f;
    for (int i = 0; i < num_octaves; ++i) {
        fbm += am * perlin_noise(point * fm);
        am  *= ampl_multiplier;
        fm  *= freq_multiplier;
    }
	return fbm;
}

// ==============================================================
// 2D turbulence

float turbulence(vec2 point) {
    float fbm = 0.0f;
    float am  = 1.0f;
    float fm  = 1.0f;
    for (int i = 0; i < num_octaves; ++i) {
        fbm += am * abs(perlin_noise(point * fm));
        am  *= ampl_multiplier;
        fm  *= freq_multiplier;
    }
	return fbm;
}

// ==============================================================
// Procedural "map" texture

const float terrain_water_level = -0.075;
const vec3 terrain_color_water = vec3(0.29, 0.51, 0.62);
const vec3 terrain_color_grass = vec3(0.43, 0.53, 0.23);
const vec3 terrain_color_mountain = vec3(0.8, 0.7, 0.7);

vec3 tex_map(vec2 point) {
    float s = perlin_fbm(point);
    return s < terrain_water_level
            ? terrain_color_water
            : mix(terrain_color_grass,
                  terrain_color_mountain,
                  s - terrain_water_level);
}

// ==============================================================
// Procedural "wood" texture

const vec3 brown_dark 	= vec3(0.48, 0.29, 0.00);
const vec3 brown_light 	= vec3(0.90, 0.82, 0.62);

vec3 tex_wood(vec2 point) {
    float t    = turbulence(point);
    float alph = (1.0f + sin(100.0f * (length(point) + 0.15f*t))) / 2.0f;
    return mix(brown_dark, brown_light, alph);
}


// ==============================================================
// Procedural "marble" texture

const vec3 white 			= vec3(0.95, 0.95, 0.95);

vec3 tex_marble(vec2 point) {
    vec2  q    = vec2(perlin_fbm(point), perlin_fbm(point + vec2(1.7f, 4.6f)));
    float alph = (1.0f + perlin_fbm(point + 4 * q)) / 2.0f;
	return mix(white, brown_dark, alph);
}

// constants for water
const float freq_multiplier_water = 11;
const float ampl_multiplier_water = 0.3;

// #### PERLIN 3D ####
// Source: https://mrl.nyu.edu/~perlin/noise/
#define N_PERM      512
#define N_PERM_HALF 256

const int permutation[N_PERM_HALF] = int[N_PERM_HALF](151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180);

int perm(int n) {
    return n >= N_PERM_HALF ? permutation[N_PERM - n] : permutation[n];
}

float grad(int hash, vec3 point) {
    // Convert lowest 4 bits of hash code into 12 gradient directions.
    int h = hash & 15;
    float u = h < 8 ? point.x : point.y;
    float v = h < 4 ? point.y : (h==12 || h==14 ? point.x : point.z);
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

#define N_CUBES 255

float perlin_noise_3d(vec3 p)
{
    // Find unit cube that contains point.
    vec3 unit_cube = floor(p);
    int xi = int(unit_cube.x) & N_CUBES;
    int yi = int(unit_cube.y) & N_CUBES;
    int zi = int(unit_cube.z) & N_CUBES;

    // Find relative x, y, z of point in cube.
    p -= unit_cube;

    // Compute fade curves for each of x, y, z.
    float u = blending_weight_poly(p.x);
    float v = blending_weight_poly(p.y);
    float w = blending_weight_poly(p.z);

    // Hash coordinates of the 8 cube corners.
    int a  = perm(xi)   + yi;
    int aa = perm(a)    + zi;
    int ab = perm(a +1) + zi;
    int b  = perm(xi+1) + yi;
    int ba = perm(b)    + zi;
    int bb = perm(b +1) + zi;

    // And add blended results from 8 orners of cube.
    return mix(mix(mix(grad(perm(aa  ), p            ),
                       grad(perm(ba  ), p-vec3(1,0,0)),
                       u),
                   mix(grad(perm(ab  ), p-vec3(0,1,0)),
                       grad(perm(bb  ), p-vec3(1,1,0)),
                       u),
                   v),
               mix(mix(grad(perm(aa+1), p-vec3(0,0,1)),
                       grad(perm(ba+1), p-vec3(1,0,1)),
                       u),
                   mix(grad(perm(ab+1), p-vec3(0,1,1)),
                       grad(perm(bb+1), p-vec3(1,1,1)),
                       u),
                   v),
               w);
}


float perlin_fbm_3d(vec3 point) {
    float fbm = 0.0f;
    float am  = 1.0f;
    float fm  = 1.0f;
    for (int i = 0; i < num_octaves; ++i) {
        fbm += am * perlin_noise_3d(point * fm);
        am  *= ampl_multiplier;
        fm  *= freq_multiplier;
    }
	return fbm;
}

float perlin_water_3d(vec3 point) {
    float water = 0.0f;
    float am  = 0.03f;
    float fm  = 0.5f;
    for (int i = 0; i < num_octaves; ++i) {
        water += am * perlin_noise_3d(point * fm);
        am  *= ampl_multiplier_water;
        fm  *= freq_multiplier_water;
    }

    while(water>0.05){
        water = 0.25*water;
    }
	return water;
}
