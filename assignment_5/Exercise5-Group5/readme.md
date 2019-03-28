# Assignment 5

## Object placement
### Planets
//TODO
### Eye
//TODO

## Rendering
### Planets
//TODO

### Spaceship
//TODO

## Keyboard events

### 8 and 9 keys
We have added two cases to the switch situated in the `Solar_viewer::keyboard()` function in order to handle the two additional key presses.
The implementation is quite trivial: we decrease (for the keypress `8`) or increase (for the keypress `9`) our variable `dist_factor_` by a constant (we fixed it at 2.0) and after that, we check that we the value remains in our bounds (2.5 < `dist_factor_` < 20.0) and clamp the value if needed.

### Arrow keys
The management for the keypresses was already provided so we only had to implement the effect of the angle on our planets.
This is done in the `Solar_viewer::paint()` function, in the case where we are not `in_ship=true` (so for the planets). We simply rotate our eye by the angles according to y and x with the `mat4::rotate_x()` and `mat4::rotate_y()` helper functions.
In order to have a correct behaviour, we have to apply the rotation along the x-axis first and then the one along the y-axis.