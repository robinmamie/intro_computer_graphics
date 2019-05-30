# Project Report - Group 5

###### Markus Ding, Robin Mamie & Charline Montial

## What is our Project About

The main goal to our project was to upgrade the already existing assignment 9 with various features, the main one being the screen-space reflections, followed by the waving water. The sky and clouds, with their reflection was our last objective.
We managed to complete all of our goals. In addition, a few improvements have been made here and there for nicer results.

## Technical Approach

The project is built and run using the usual way of this semester, by creating a build folder, and executing inside:

    cmake ..
    make -j
    ./terrain

### Screen-space Reflections

_Most of the work for this part was done in the files `src/render/MeshViewer.cpp`, `src/render/FrameBuffer.[(cpp)|h]` and `src/shader/reflection.frag`._

At first, we had to accommodate the code in order to communicate the colors of the terrain to the reflection shader.
For that, we created the class `FrameBuffer`, that contains all the informations necessary to generate simple screen-space reflections.
Namely, we forward the depth of the pixel and its color to the other shader, using frame buffers.
Even though we used screen buffers in assignment 7, using two of them as output of the same shader was not trivial, and we spent a lot of time looking for online resources talking about this.
After stringing bits and pieces of information, we finally managed to get a fully working implementation of this idea.
We also learnt the hard way that online OpenGL support is either scarce or very specialised...

Then, we wanted to adapt the code provided by one of the links displayed on the page reserved for the project ideas, namely [this blog post](https://gizmosandgames.com/2017/01/17/screen-space-reflections/).
Unfortunately for us, after a few days of trying, we did not manage to adapt the algorithm in our code, to our dismay.
No visible results were convincing and/or made sense.

Therefore, we decided to work out an even simpler algorithm, that uses a trivial ray marching technique.
Indeed, using the ray reflected from our viewing ray - according to the normal of the water -, we just iterate over very small portions of it until the resulting ray (i.e. viewing ray + `i` number of small reflection rays) "hit" the terrain, i.e. its depth is greater than the terrain.
The latter was sampled in a frame buffer, and can be directly used in the shader for this purpose.
The search is abandoned if the ray is outside of the screen or it "hits" the sky, meaning there will never be terrain after that.

There is a small exception to the screen rule.
To make the reflections look nicer, we still output a color when the ray exits the screen horizontally, mening they are not strictly speaking "screen-space" reflections.
But by clipping the result back to the border of the screen, it offers very beautiful results when the terrain is not perfectly flat, which is the case in this project.

![Reflections on still water](images/still_reflection_wo.png){width="600px"}

![Adjusted reflections on still water](images/still_reflection.png){width="600px"}

![Adjusted reflections on waving water](images/wavy_reflection.png){width="600px"}


### Dynamic Water

### Sky and Clouds

### Minor Improvements

#### Sun Shimmering on the Water

We wanted to amplify the effect of the phong lighting model in order to have a bigger effect of the sun shimmering on the water.
This was simply done by, instead of the "normal" colour of the sky, outputting plain white when the dot product between the reflected ray and the light source was greater than 0.8.
This value was empirically computed, and gave us the most convincing results.

#### Sky Reflection on the Water

As specified in our contract, we wanted to add the reflections of the sky.
We have done it, but we decided to not include it in the final presentation.
Unfortunately, the clouds we use have a frequency too high for the reflections to look good, as showcased by the following examples:

![Example of sky reflection](images/sky_reflection1.png){width="600px"}

![Example of sky reflection](images/sky_reflection2.png){width="600px"}

The sky reflections are even weirder when the water is moving, and the whole landscape just looks like a giant ugly vortex.
As proof for our work, we have included the shader under the name `src/shader/cursed_reflection.frag`.

#### Separation of water and land meshes

_Most of the work for this part was done in the files `src/render/MeshViewer.[(cpp)|h]` and `src/main_terrain.cpp`._

We wanted to separate the land and water meshes in our project for two reasons.

First, the transition between the land and the water was weird as it can be seen in the screenshot below and it certainly did not improve when the water began to wave.

Secondly, we wanted to achieve this because it would allow us to have a transparent water. We thought that it would be more realistic to see the bottom of our land and for that, we needed to separate the two meshes, which is what we did.

![The transition between the land and the water was not very nice when the land and the water were rendered using a single mesh.](images/bad_transition.png){width="600px"}

To achieve this, we added two attributes to the `MeshViewer` class for the water, namely a shared pointer to an instance of the class `Mesh` and another one to an instance of the class `StaticMeshActor`. Then, the water mesh is also generated in the file `main_terrain.cpp` in the same way as the land mesh, but using its own function to generate the wanted values for the water.

#### Sides of the terrain closed

_Most of the work for this part was done in the files `src/render/MeshViewer.[(cpp)|h]` and `src/main_terrain.cpp`._

We wanted to close the sides of the terrain because we could see the water under the mountain and it was less pleasant to see.

To achieve this goal, we added another mesh similarly to the water and passed the correct values to it.

Since we only need the sides for our mesh, we were allowed to have an array of vertices of a size much smaller than the one needed to generate the land and the water.

To fill in the array, we fix one coordinate and iterate on the other to fill one wall. We repeat the procedure four times to fill in the array and push the newly constructed faces.
We alternate between vertices having the z coordinate at the height of the terrain and another one being at the base of our wall. We can then construct our faces using the vertices which follow the terrain and the ones being aligned at the bottom of the terrain.

The ground has also been implemented and was simpler since it did not need the height map.

For a more good-looking result, Phong has been disabled for the sides.

#### Miscellaneous

The user can dump all frames in files by pressing `C`.

## Results

![Screen space reflections + Phong + Sky and clouds](images/1.png){width="600px"}

![Closer look at screen space reflections](images/2.png){width="600px"}

![Closer look at the sun shimmering on the water](images/3.png){width="600px"}

![Closer look at the side walls of the terrain](images/4.png){width="600px"}

<video class='center' width="1000" controls>
  <source src="video/sample.mp4" type="video/mp4">
</video>
*Demonstration video starring all the implemented features*

## Contributions

#### Screenspace reflections
Robin Mamié

#### Dynamic water
Markus Ding, Robin Mamié & Charline Montial

#### Sky and clouds
Markus Ding & Charline Montial

#### Sky and clouds reflections
Robin Mamié

#### Sides of the terrain closed
Robin Mamié & Charline Montial

#### Separate land and water mesh
Charline Montial

#### Sun shimmering on the water
Robin Mamié

### Total
Robin Mamié: 38%
Markus Ding: 31%
Charline Montial: 31%

## References
