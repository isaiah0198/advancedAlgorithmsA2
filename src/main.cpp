#include <iostream>
#include "Graph.hpp"
#include "StoerWagner.hpp"

int main() {
    Graph graph(3);

    graph.addEdge(0, 2, 2);
    graph.addEdge(1, 2, 4);
    graph.addEdge(0, 1, 3);

    int cut = stoerWagnerMinCut(graph);
    std::cout << "Minimum cut: " << cut << '\n';

    return 0;
}