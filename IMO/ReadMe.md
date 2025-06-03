# Term 8: IMO (Intelligent Optimization Methods)

This repository contains a comparison of multiple methods that can be used to solve a modified TSP problem, in which we construct two separate paths, each containg half of the vertices.

The test instances are taken from [TSPLib](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/) and the results are visualized using [matplotlib-cpp](https://github.com/lava/matplotlib-cpp/blob/master/matplotlibcpp.h).

### Authors

Mikołaj Nowak, Anna Roszkiewicz

### Running the code

To build the project, do:

```bash
make PYTHON_VERSION={your-python-version}
```

And then run it with:

```bash
./tsp-study TSPlib95/{test-file}.tsp
```

Note: To run the code with `matplotlib-cpp`, you need to have `python3-dev` installed.
