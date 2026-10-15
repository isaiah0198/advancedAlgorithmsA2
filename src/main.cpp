#include <iostream>
#include "Graph.hpp"

int main() {
    Graph graph(3);

    graph.addEdge(0, 2, 2);
    graph.addEdge(1, 2, 4);

    graph.mergeVertices(0, 1);

    std::cout << "0 -> 2 weight: " << graph.getWeight(0, 2) << '\n';
    std::cout << "2 -> 0 weight: " << graph.getWeight(2, 0) << '\n';

    std::cout << "Vertex 0 active: " << graph.isActive(0) << '\n';
    std::cout << "Vertex 1 active: " << graph.isActive(1) << '\n';
    
    return 0;
}