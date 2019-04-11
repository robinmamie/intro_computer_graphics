# Assignment 6

## Building View and Projection Matrices for a Cube Face

To implement the function m_constructLightProjectionMatrix, we used the perspective function, with, as the field of view y value: 90 (degrees), the ratio : 1 as the cube is centered and symmetric, and the distance with 0.1 and 6 as provided.

For the m_constructLightViewMatrix function, each face of the cube had to be processed in a separate case; the function look-at has been used, and an eye, center and up vector have been computed according to each face.

## Writing the Fragment Shaders

In the file shadowmap_gen.frag, the ray intersection distances is simply the length of the vector since, in eye coordinate, the light is at the origin.

About the file phong_shadow.frag, [TO COMPLETE]

# Setting the Blend Function

[TO COMPLETE; JE L'AI FAIT JUSTE CAR JE ME SUIS RENSEIGNEE MAIS JE NE SAIS PAS EN QUOI C'EST JUSTE HELAS]

## Contributions

Ding Markus 31%
Mamie Robin 31%
Montial Charline 38%
