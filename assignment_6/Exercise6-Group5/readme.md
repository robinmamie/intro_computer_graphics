# Assignment 6

## Billboard

To get a pleasing result, first we need to display the billhoard, which is done using the color shader, the texture and the draw function as explained in the provided comments.

To obtain a progressive transparency, Pythagore's theorem has been used to compute the distance. Indeed,  the further from the origin the current pixel is, the stronger the transparency. We have simply done a linear interpolation of the alpha values. Also, inside the sun, no colour is rendered because it is not needed (we used start drawing the glow at 0.9\*radius of the sun because if we started at the radius exactly, we would have some small graphic artifacts due to rounding errors).

For all the comparisons and distance computations, we did not compute the square roots (so it is not the full pythagoras theorem) in order to optimize the compuattion costs. So all of our distances are in fact squared distances but as they all are, it does not create any problems.

Additional offsets and custom values have been added to have a nice result that looks like the provided pictures in the provided pdf.

To achieve that the billboard constantly faces the camera, we used basic trigonometry to compute the rotation angles (so the angles between our vector between the camera and the billboard position and the x and y axis).
As we are using a normalized vector, its norm (so the hypotenuse if we draw the triangles between our vector and the two axis) equals 1 so we could simply omit it in the arcsin function (because multiplying by 1 does not change anything).

One last thing had to be done : since the arcsin(x) function maps to [- π/2, + π/2] but we need the following output interval: [0, 2π] to be able to turn the camera fully around the sun (or any other planet). In order to solve this issue, we took into account the sign of the z coordinate in our x-angle computation.

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
