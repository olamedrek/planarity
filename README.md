# Planarity

This project allows to test whether a graph is planar in complexity O(n^2). It can also display planar embeddings
of biconnected graphs.

## Usage

Compile project using cmake (the required version is 3.6). Create an input file with representation of a graph to be tested.
Run program using the following command:

```
python run.py -exe [exe_path] -input [input_path] [-draw]
```

### Input

The first line of input should contain two space-separated integers 1 ≤ n, 0 ≤ m - the number of vertices and the number of edges.
The next m lines describe the edges of the graph: each of them should contain two space-separated integers 1 ≤ u,v ≤ n - the vertices that form an edge.

### Output

The first line of the output contains information about biconnectivity of the graph, the second line contains informatio about its planarity. If the flag -draw is set, the image of a graph is displayed.

## Running the tests

Script run_all.sh runs all the tests in test directory and compares results to expected results stored in .exp files. To run this script:

```
./run_all.sh [exe_path]
```