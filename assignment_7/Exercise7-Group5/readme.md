# Assignment 7

## Building View and Projection Matrices for a Cube Face

To implement the function m_constructLightProjectionMatrix, we used the perspective function, with, as the field of view y value: 90 (degrees), the ratio : 1 as the cube is centered and symmetric, and the distance with 0.1 and 6 as provided.

For the m_constructLightViewMatrix function, each face of the cube had to be processed in a separate case; the function look-at has been used, and an eye, center and up vector have been computed according to each face.

## Writing the Fragment Shaders

In the file shadowmap_gen.frag, the ray intersection distances is simply the length of the vector since, in eye coordinate, the light is at the origin.

About the file phong_shadow.frag, [TO COMPLETE]

# Setting the Blend Function

As can be seen in the [documentation](https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glBlendFunc.xml) for the `glBlendFunc()` function, its two parameters are the scaling methods for the source and destination RGBA values. As we do not want to do anything other than adding the specular and diffuse contributions, we are using the constant `GL_ONE` for both parameters.

And of course, we need to enable the blending property with the line `glEnable(GL_BLEND);` as it is disabled by default.

## Contributions

Ding Markus 25%
Mamie Robin 37%
Montial Charline 38%
