# Advanced Algorithms A2

## Project Overview

This project compares two algorithms for the **global minimum cut** problem:

- **Stoer-Wagner** — deterministic minimum cut
- **Karger’s Randomised Contraction Algorithm** — probabilistic minimum cut

The project investigates how graph size, graph density, and Karger repetition count affect:

- runtime
- reliability
- probability of finding the true minimum cut

The implementation is written in **C++17**.

---

## Project Structure

```text
advancedAlgorithmsA2/
├── include/
│   ├── Graph.hpp
│   ├── Karger.hpp
│   └── StoerWagner.hpp
│
├── src/
│   ├── Graph.cpp
│   ├── Karger.cpp
│   ├── StoerWagner.cpp
│   └── main.cpp
│
├── results/
│   └── benchmark.csv
│
├── CMakeLists.txt
└── README.md
```

---

## Algorithms

### Stoer-Wagner

Stoer-Wagner is a deterministic algorithm for finding the global minimum cut of an undirected weighted graph.

The implementation uses:

- an adjacency matrix
- active vertex tracking
- maximum-adjacency phases
- vertex contraction

The implementation used in this project has approximately:

```text
Time:  Θ(n^3)
Space: Θ(n^2)
```

---

### Karger

Karger’s algorithm repeatedly contracts randomly selected edges until only two supervertices remain.

A single run is not guaranteed to find the global minimum cut, so the project also includes a repeated version that executes Karger multiple times and returns the smallest cut found.

The implementation uses:

- `std::mt19937` for reproducible randomisation
- weighted edge selection
- adjacency-matrix contraction
- configurable repetition counts

Because this implementation scans the adjacency matrix during random edge selection, one Karger run is approximately:

```text
Time:  Θ(n^3)
Space: Θ(n^2)
```

With `r` repetitions:

```text
Time: Θ(rn^3)
```

---

## Building with CMake

From the repository root:

```bash
cmake -S . -B build
cmake --build build
```

Run the program with:

```bash
./build/advancedAlgorithmsA2
```

The program should be run from the repository root so that:

```text
results/benchmark.csv
```

is written to the correct location.

---

## Manual Build

The project can also be compiled directly with:

```bash
g++ -std=c++17 -O2 \
src/main.cpp \
src/Graph.cpp \
src/StoerWagner.cpp \
src/Karger.cpp \
-Iinclude \
-o graph_test
```

Run with:

```bash
./graph_test
```

---

## Debug Build

AddressSanitizer was used during development:

```bash
g++ -std=c++17 -fsanitize=address -g \
src/main.cpp \
src/Graph.cpp \
src/StoerWagner.cpp \
src/Karger.cpp \
-Iinclude \
-o graph_test
```

This build was used for debugging only and not for final performance measurements.

---

## Experiments

The program performs several experiments.

### Known-answer tests

The algorithms are checked using small graphs with known minimum cuts:

- weighted triangle
- path
- cycle
- five-node weighted graph

### Karger repetition reliability

Karger is tested using:

```text
1, 2, 5, 10, 20, 50 repetitions
```

### Graph size scaling

Random connected graphs are tested with:

```text
10, 25, 50, 100 vertices
```

### Density experiment

For 50-vertex graphs, the tested densities are:

```text
0.10
0.25
0.50
0.75
```

### Multi-graph reliability

Five graph-generation seeds are used:

```text
42
142
242
342
442
```

This reduces dependence on a single randomly generated graph.

### Repetitions across density

Karger is compared using:

```text
1
10
20
```

repetitions across each graph density.

---

## Benchmark Methodology

Final benchmark runs use:

```text
C++17
-O2 optimisation
```

Runtime is measured using:

```cpp
std::chrono::high_resolution_clock
```

Each timing condition is executed **51 times**, and the **median** runtime is reported.

Random seeds are fixed so experiments can be reproduced.

---

## Results

Benchmark results are exported to:

```text
results/benchmark.csv
```

The CSV includes results for:

- repetition reliability
- repetition runtime
- graph size scaling
- graph density
- multi-graph reliability
- Karger repetition count across densities

---

## Key Findings

The main observations from the experiments were:

- Stoer-Wagner was faster than repeated Karger in the tested implementation.
- Both implementations showed approximately cubic scaling with graph size.
- Karger runtime increased approximately linearly with repetition count.
- Karger reliability improved substantially with repeated execution.
- Observed Karger success probabilities were much higher than the classical worst-case bound.
- Increasing graph density was associated with lower Karger success rates in the generated graph family.
- Density had relatively little effect on runtime because both algorithms use an adjacency matrix.

These findings apply to the implementations and graph families used in this project and should not be interpreted as universal performance claims.

---

## Reproducibility

To reproduce the benchmark:

```bash
cmake -S . -B build
cmake --build build
./build/advancedAlgorithmsA2
```

Then inspect:

```bash
cat results/benchmark.csv
```

---

## Author

**Isaiah Lane**

Primary author for:

- Stoer-Wagner implementation
- Karger implementation
- experimental design
- benchmarking
- analysis