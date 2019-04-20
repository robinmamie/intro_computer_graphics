# Assignment 8

## Commands

**Create directories that are empty so not on github**:
```bash
cd assignment_8
mkdir data_out
mkdir reverse_in
mkdir reverse_out
```

**Initial setup**:
```bash
cd assignment_8/build
cmake ..
```

**Compilation**:
```bash
cd assignment_8/build
make
```

**Run single file**:

In this case for `plant_01.json`, it generates two output files inside the `data_out` folder, one `.svg` file which is the drawing of that element and one `.txt` file which corresponds to the final expansion of the sequence of symbols (all parameters are defined in the `.json`).
```bash
cd assignment_8
./build/lsys data_in/plant_01.json data_out/plant_01
```

**Run on all input files**:

For all deterministic systems (in `data_in` and writes results in `data_out`):
```bash
cd assignment_8/
./run_deterministic.sh
```

For all stochastic systems (in `data_in` and writes results in `data_out`):
```bash
cd assignment_8/
./run_stochastic.sh
```

For all reverse files (in `reverse_in` and writes results in `reverse_out`):
```bash
cd assignment_8/
./run_reverse.sh
```

## Useful tips

**Converting svg to png files**:

With [imagemagick](!https://www.imagemagick.org/):
```bash
convert a.svg a.png
```

With [Inkscape](!https://inkscape.org/en/)
```bash
inkscape a.svg --export-png=a.png
```
