#version 430

in vec3  v2f_ec_vertex;
in vec3  v2f_normal;
in float v2f_height;

out vec4 f_color;

//uniform sampler2D gColor;
uniform sampler2D color_map;
uniform sampler2D depth_map;
uniform vec2 resolution;

const float terrain_water_level = -0.03125 + 1e-6;
const vec4 sky_color = vec4(0.8f, 1.0f, 1.0f, 1.0f);


// Consts should help improve performance
const float rayStep = 0.25;
const float minRayStep = 0.1;
const float maxSteps = 20;
const float searchDist = 5;
const float searchDistInv = 0.2;
const int numBinarySearchSteps = 5;
const float maxDDepth = 1.0;
const float maxDDepthInv = 1.0;


const float reflectionSpecularFalloffExponent = 3.0;


uniform mat4 projection;


vec3 BinarySearch(vec3 dir, inout vec3 hitCoord, out float dDepth)
{
    float depth;


    for(int i = 0; i < numBinarySearchSteps; i++)
    {
        vec4 projectedCoord = projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;


        depth = texture2D(gPosition, projectedCoord.xy).z;


        dDepth = hitCoord.z - depth;


        if(dDepth > 0.0)
            hitCoord += dir;


        dir *= 0.5;
        hitCoord -= dir;    
    }


    vec4 projectedCoord = projection * vec4(hitCoord, 1.0); 
    projectedCoord.xy /= projectedCoord.w;
    projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;


    return vec3(projectedCoord.xy, depth);
}


vec4 RayCast(vec3 dir, inout vec3 hitCoord, out float dDepth)
{
    dir *= rayStep;


    float depth;


    for(int i = 0; i < maxSteps; i++)
    {
        hitCoord += dir;


        vec4 projectedCoord = projection * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;


        depth = texture2D(gPosition, projectedCoord.xy).z;


        dDepth = hitCoord.z - depth;


        if(dDepth < 0.0)
            return vec4(BinarySearch(dir, hitCoord, dDepth), 1.0);
    }


    return vec4(0.0, 0.0, 0.0, 0.0);
}


void main()
{
    vec2 gTexCoord = gl_FragCoord.xy * gTexSizeInv;
    vec2 gTexCoord = ( gl_FragCoord.xy / resolution.xy );


    // Samples
    float specular = texture2D(gColor, gTexCoord).a;


    if(specular == 0.0)
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        return;
    }


    vec3 viewNormal = normalize(v2f_normal) * -sign(dot(v2f_normal, v2f_ec_vertex));
    vec3 viewPos    = v2f_ec_vertex;


    // Reflection vector
    vec3 reflected = normalize(reflect(normalize(viewPos), normalize(viewNormal)));


    // Ray cast
    vec3 hitPos = viewPos;
    float dDepth;


    vec4 coords = RayCast(reflected * max(minRayStep, -viewPos.z), hitPos, dDepth);


    vec2 dCoords = abs(vec2(0.5, 0.5) - coords.xy);


    float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);


    // Get color
    gl_FragColor = vec4(texture2D(gEffect, coords.xy).rgb,
        pow(specular, reflectionSpecularFalloffExponent) *
        screenEdgefactor * clamp(-reflected.z, 0.0, 1.0) *
        clamp((searchDist - length(viewPos - hitPos)) * searchDistInv, 0.0, 1.0) * coords.w);
}
