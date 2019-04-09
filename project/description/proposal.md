---
title: Procedural generation of the caribbean alps
---

![A nice mountainy landscape](images/mountains.jpg){width="600px"}

# Description

In this project, we want to generate a landscape procedurally. We will be using [Perlin Noise](https://en.wikipedia.org/wiki/Perlin_noise) in order to create a mountainy landscape.

# Goals and Deliverables

Voir: https://lgg.epfl.ch/teaching/ICG2019/icg_lectures/2019_project_instructions/

##Minimal requirements

A movable camera (with arrow keys) that allows to move around the world that is generated proceduraly and resembles a mountain-like landscape.

##Optional extensions

- More complex lighting model (Phong)

- Water with reflections (or only specular component) on it

- Virtual clouds which create shadows on the ground, no volumetric clouds, simply a monoscale texture that blocks or lets light through. Maybe with a basic skybox(?)

<p align="center"> ![Something light this](images/clouds.jpg){width="300px"} </p>

- Multiple light sources: the sun high above and maybe some smaller and less intense light sources on the map itself.

<p align="center"> ![Example of additional lights (the lamps)](images/light.jpg){width="300px"} </p>

##Expected results

We want to be able to freely move the camera around the world in order to explore it. We should be able to see the different biomes of the mountains and the different lighting effects (reflections, shadows, specular, etc) depending on which optional extensions we will be required to implement.
There should be a fixed rendering distance beyond which the world is not generated. When moving, the new parts of the world should be generated on the fly.

# Schedule

We would prefer to specify at the begining of each week on what which team member will work. But we were thinking about the following general schedule:

- Week 1: Perlin noise function and mesh generation for the terrain
- Week 2: Create different biomes (water, ice, rock, etc) and map textures
- Week 3: Add phong lighting model (ambient, diffuse, specular)
- Week 4: Add shadows created by clouds or multiple light sources(?)

# Resources

OpenGL and ?

___
# Links [à supprimer avant de submit]
- [General guideline for map generation](https://www.redblobgames.com/maps/terrain-from-noise/)
- [Perlin Noise implementation c++](https://github.com/sol-prog/Perlin_Noise)