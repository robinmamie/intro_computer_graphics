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


float perlin_fbm(vec2 point) {
    float fbm = 0.0f;
    float am  = 1.0f;
    float fm  = 1.0f;
    for (int i = 0; i < num_octaves; ++i) {
        fbm += am * perlin_noise(1.25*point * fm);
        am  *= ampl_multiplier;
        fm  *= freq_multiplier;
    }
	return fbm;
}

// constants for water
const float freq_multiplier_water = 11;
const float ampl_multiplier_water = 0.3;

float perlin_water(vec2 point) {
    float water = 0.0f;
    float am  = 0.03f;
    float fm  = 0.5f;
    for (int i = 0; i < num_octaves; ++i) {
        water += am * perlin_noise(point * fm);
        am  *= ampl_multiplier_water;
        fm  *= freq_multiplier_water;
    }

    while(water>0.05){
        water = 0.25*water;
    }
	return water;
}

