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
//TODO

## Rendering
### Planets
The planets could have been rendered almost like the sun, the only difference is to choose the correct planet instance. Thus, to have a clearer code, the code rendering the sun has been extracted as a method which takes a Planet as argument as well as other parameters.

### Spaceship
The only difference with the planets is that we can draw the ship using the function draw(). We also had to use its own texture.

## Keyboard events
//TODO
