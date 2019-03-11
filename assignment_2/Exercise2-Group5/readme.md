# Assignment 2

No major difficulty was encountered during the implementation of this part, thanks to crystal clear instructions and a good team work.

1) Ambient:
We simply needed to change the color that is used, we replaced the normal color by ambient one which is the multiplication of the ambiance lightening and the material properties.

2) Diffuse:
We had to compute the contribution of each light ray which obviously depends on its direction (the difference between the light source position and the point for which we are computing things).
We are only considering the diffuse light that is going outside of the spheres (so in the direction of the normal) as the one going into the object is useless to us, we are checking the direction of the diffuse light by verifying if the dot product is positive (as mentioned in assignment 1).

3) Specular:
Once again, we had to consider all light rays. In order to compute their specular reflection, we simply applied the formula seen in class: [I = I_l * m_s(r⋅v)^s](https://lgg.epfl.ch/teaching/ICG2019/icg_lectures/03-Lighting.html#/specular-reflection-4) with [r = 2n(n⋅l) − l](https://lgg.epfl.ch/teaching/ICG2019/icg_lectures/03-Lighting.html#/specular-reflection-2) which implementation is trivial with the helper function in the vec3 class.
At the end, we must make sure that the two conditions n⋅l≥0 and n⋅r≥0 are satisfied, otherwise there is no illumination.


4) Shadows:
We simply had to compute a shadow ray which is the same as what we needed to implement in the first assignment but if there is an intersection between our shadow ray and any other object, then we know that there will be a shadow in which case we do not compute the diffuse and specular elements.
We needed to add a small offset in order to counteract the precision problems (due to the floating point representation) which cause the "shadow acne".
There was one additional point that we forgot to consider at first. Indeed, if an object placed on the other side of the light source intersected with the ray, it would create a shadow on the starting point. We resolved that by thresholding the distance of the ray with the distance of the light source with respect to the starting point.


5) Reflections:
In order to be able to reuse the trace() function, we must first update our light ray, it is no longer the initial one we used but the projection of the initial ray onto the reflection axis (the normal one).
Once the function call is done, we only have to implement the linear interpolation which is quite trivial:
color = ( 1 − α ) · color + α · reflected_color

Obviously, the color used for the interpolation is the sum of all previous elements (ambient, diffuse, specular).

Ding Markus 30%
Mamie Robin 35%
Montial Charline 35%
