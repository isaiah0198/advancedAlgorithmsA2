#include <iostream>
#include "Graph.hpp"

int main() {
    Graph graph(3);

    graph.addEdge(0, 2, 4);

    std::cout << "Vertex 0 active: " << graph.isActive(0) << '\n';
    std::cout << "Vertex 1 active: " << graph.isActive(1) << '\n';
    std::cout << "Vertex 2 active: " << graph.isActive(2) << '\n';
    
    return 0;
}