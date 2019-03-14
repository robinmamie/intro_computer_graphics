# Assignment 3

No major difficulty was encountered during the implementation of this part.

## 1. Vertex Normals
To complete this part using the efficient way, we had to iterate over all triangles, get their angle weights and add their contribution to their respective vertices.

## 2. Ray-triangle Intersection
In order to compute the intersections and solve the equation which lead to them, Cramer has been used as advised. The solutions of the system have only been used if the draw_mode was not FLAT as requested.

## 3. Efficient Ray-Mesh Intersections
To check if the ray was in the bounding box or not, we have implemented the following: we considered the box as the inner-volume delimited by three pairs of parallel planes, which, all together, form the sides of the cube.
Then, the ray is cut wherever it does not belong to the volume of the cube and if no piece of the ray remains, we know that it does not intersect with the box and otherwise, it surely does.

## 4. Parallelization


Ding Markus 30%
Mamie Robin 40%
Montial Charline 30%
