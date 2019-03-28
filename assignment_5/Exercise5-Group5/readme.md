# Assignment 5

## Object placement
### Planets
For the planets (except the moon), to place them, we simply need to translate
the planet from the origin to its distance before applying the rotation.

The moon is a special case since it has to turn around the earth.
To take care of it correctly, we have first done the same as the other planets and we added a final translation to the center of the earth.

## Camera
### Observing the planets
First, the camera behaves differently if the ship view is selected or not.
If we are not in the ship, we first translate to the distance factor to avoid being in the planet. Then we rotate if the arrows keys have been pressed to finally translate to the center of the planet we want to look at.

### In spaceship mode
The main difference comapred to the planets is that we do not rotate along the x-axis considering the `x_angle_` but that we have a fixed angle which allows us to see the spaceship from behind and above.
For the y-axis, we have to take into account the angle of the spacehip in addition to the `y_angle` in order to always have the tip of the spaceship in front of us.
Due to the size of the spaceship, we have also added an additional scaling factor to `dist_factor_` in order to be farther away and to see the entire spaceship.

## Rendering
### Planets
The planets could have been rendered almost like the sun, the only difference is to choose the correct planet instance. Thus, to have a clearer code, the code rendering the sun has been extracted as a method which takes a Planet as argument as well as other parameters.

### Spaceship
The only difference with the planets is that we can draw the ship using the function draw(). We also had to use its own texture.

## Keyboard events

### 8 and 9 keys
We have added two cases to the switch situated in the `Solar_viewer::keyboard()` function in order to handle the two additional key presses.
The implementation is quite trivial: we decrease (for the keypress `8`) or increase (for the keypress `9`) our variable `dist_factor_` by a constant (we fixed it at 2.0) and after that, we check that we the value remains in our bounds (2.5 < `dist_factor_` < 20.0) and clamp the value if needed.

### Arrow keys
The management for the keypresses was already provided so we only had to implement the effect of the angle on our planets.
This is done in the `Solar_viewer::paint()` function, in the case where we are not `in_ship=true` (so for the planets). We simply rotate our eye by the angles according to y and x with the `mat4::rotate_x()` and `mat4::rotate_y()` helper functions.
In order to have a correct behaviour, we have to apply the rotation along the x-axis first and then the one along the y-axis.

## Contributions
Ding Markus 30%
Mamie Robin 30%
Montial Charline 40%

