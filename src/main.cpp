#include <iostream>
#include "Graph.hpp"

int main() {
    Graph graph(3);

    graph.addEdge(0, 2, 4);

    std::cout << "0 -> 2 weight: " << graph.getWeight(0, 2) << '\n';
    std::cout << "2 -> 0 weight: " << graph.getWeight(2, 0) << '\n';

    return 0;
}