## Notes

While testing the `Graph` class, I added an edge between vertices `0` and `2` with weight `4`.

The test output was:

```text
0 -> 2 weight: 4
2 -> 0 weight: 0

This showed that the adjacency matrix was not symmetric, even though the graph is meant to be undirected.
The problem was in addEdge(). I had only written:
weights_[u][v] = weight;

so the reverse direction was never updated.
For an undirected graph, the invariant should be:
weights_[u][v] == weights_[v][u]

I fixed the implementation by also adding:
weights_[v][u] = weight;

After recompiling and rerunning the same test, both directions should return 4.
What I learned
A program compiling successfully does not mean the graph representation is logically correct. Small tests that check invariants, such as matrix symmetry for an undirected graph, can expose errors before they become much harder to trace inside the minimum-cut algorithms.

Added isActive() to Graph.cpp and used it in main.cpp, but forgot to declare it in Graph.hpp. The compiler reported both that Graph had no isActive member and that the out-of-line definition did not match a declaration. Fixed by adding the method declaration and active_ member to the header.

For an undirected adjacency matrix, after calculating a new contracted edge weight once, mirror that result rather than independently modifying both entries.

### Stoer-Wagner inactive vertex selection bug

While implementing the maximum-adjacency phase, the program began crashing with a segmentation fault.

I compiled with AddressSanitizer:

```bash
g++ -std=c++17 -fsanitize=address -g \
src/main.cpp src/Graph.cpp src/StoerWagner.cpp \
-Iinclude -o graph_test

AddressSanitizer showed the crash occurred when writing to the added vector.
I added temporary output for the selected vertex and found that vertex 1 was being selected even though it had previously been marked inactive.
The selection condition was:
if (graph.isActive(v) || added[v]) {
    continue;
}

This incorrectly skipped active vertices and allowed inactive vertices to be considered.
I corrected it to:
if (!graph.isActive(v) || added[v]) {
    continue;
}