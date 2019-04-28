# Assignment 8

## Grammar expansion

The implementation is quite trivial, we simply look at the mapping `(key: char, value: expansion)` and if the keys contain the character, we return the corresponding expansion and otherwise the character itself.

For the functions `LindenmayerSystem::expandOnce()` and `LindenmayerSystem::expand()`, we simply loop over all characters or respectively applying the expansions several times.

## Drawing

We separated the different actions (rotation on `+` and `-`, new branch on `[`, end of branch on `]`, add a segment on `F` and any other character) with the help of a switch statement.

Each branch is composed of pairs of `vec2` which are the Segments that compose it (each pair is the starting point of a segment and its ending point).
However, the first pair of each branch is not a Segment that we should draw as it contains the information about the branch itself, so its position and direction.

### Creating a branch

We create a new `std::Vector<Segment>` with a first "Segment" being its initial position and direction as described above and we add the newly created branch to our stack of branches.

### Resolving a branch

We copy all Segments contained in the branch except the first one (as it is its position and direction) and add them to the list of Segments to draw that will be returned at the end of the function after all branches are resolved.

### Rotation

We simply apply the [rotation matrix](https://en.wikipedia.org/wiki/Rotation_matrix) to our direction vector and update its value in the corresponding branch.

### Adding a segment

We simply add a new Segment to the current branch which is the pair `(position, position+direction)` (because the direction is normalized) and update the value of the position of the branch to the new one.

## Reverse-Engineering

This was simply about recognizing patterns, seing where the recursion was originating and how the models were evolving step by step.
Thanks to the provided drawings, it was easy to check that our rules are indeed correct and behaving as expected.

## Stochastic systems

We reused the `LindenmayerSystemDeterministic::expandSymbol()` function that we implemented previously and changed a few details in order to take into account a probability generated with `dice.roll()` so that we have some randomness and not always the same behaviour.

## Contributions

Ding Markus 40%
Mamie Robin 30%
Montial Charline 30%
