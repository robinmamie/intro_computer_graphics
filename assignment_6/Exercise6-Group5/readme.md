# Assignment 6

## Billboard

To get a pleasing result, first we need to display the billhoard, which is done using the color shader, the texture and the draw function as explained in the provided comments.

To obtain a progressive transparency, Pythagore's theorem has been used. Indeed,  the further from the origin the current pixel is, the stronger the transparency. Also, inside the sun, no colour is rendered because it is not needed.

Additional offsets and custom values have been added to have a nice result that looks like the provided pictures in the provided pdf.

To achieve the the billboard to constantly face the camera, we used trigonometry to find rotation angles.

One last thing had to be done : since arcsin(x) function maps to [- π/2, + π/2] but we need the following output interval: [0, 2π] to be able to turn the camera fully. To solve this problem, we took account of the sign of the z coordinate.

## Phong lighting model

About the Phong lighting model, the normal vectors had to be transformed according to the eye, which results in the inverse of the transposed view matrix. Then, the color shaders have to be used along with the phong shader when the planets and the ship are rendered.

In the phong.frag file, the phong model has been implemented in a very similar way that in previous assignments.

In the phong.vert file, the vertex position, normal and light position on the eye space have been computed and the final vertex position has been correctly assigned to the OpenGL variable, gl_Position.

## Shading the Earth

For the Earth, the only major differences are in the file earth.frag, where the colors have to be mixed according to the day and night cycle.

Also, each contribution (Diffuse and Specular) has been coded with its respective conditions, and the gloss variable has been added to have the Specular effect only when we have the water.

## Contributions

Ding Markus 37%
Mamie Robin 38%
Montial Charline 25%
