# Assignment 1

Apart from the derivations explained in `TheoryExercise.pdf`, the core of our work was to understand how the other files were used by the raytracer (e.g. in `Sphere.cpp`, as advised by the instructions).

Concerning the ray-plane intersection, we have simply implemented the equation which was given in the slides. The intersection normal was trivial in this case.

For the ray-cylinder intersection and the normal derivations (theory exercise), please refer to the attached file `TheoryExercise.pdf`.

The cylinder class could be implemented without any trouble thanks to the provided example of the sphere. The intersection was also not difficult to implement once the theory part was done.

To consider the fact that the cylinder is finite, we used the Pythagorean theorem to check if the intersection point was not beyond the scope of the cylinder (cf. `TheoryExercise.pdf`). The equation for the normal vectors is also explained in this file.

The only difficulty encountered was how to point the normal of the cylinder towards us depending on which side the intersection was. We have not directly thought of using the dot product to check if the angle was acute or not. Once we found this, the problem was solved.

Ding Markus 35%
Mamie Robin 30%
Montial Charline 35%
