#include <iostream>
#include "Graph.hpp"
#include "StoerWagner.hpp"
#include "Karger.hpp"

int main() {
    Graph triangle(3);
    triangle.addEdge(0, 1, 3);
    triangle.addEdge(0, 2, 2);
    triangle.addEdge(1, 2, 4);


    std::cout << "Karger triangle cut: "
              << kargerMinCut(triangle, 42)
              << '\n';


    Graph path(4);
    path.addEdge(0, 1, 1);
    path.addEdge(1, 2, 1);
    path.addEdge(2, 3, 1);

    std::cout << "Path min cut: "
              << stoerWagnerMinCut(path)
              << " (expected 1)\n";


    Graph cycle(4);
    cycle.addEdge(0, 1, 1);
    cycle.addEdge(1, 2, 1);
    cycle.addEdge(2, 3, 1);
    cycle.addEdge(3, 0, 1);

    std::cout << "Cycle min cut: "
              << stoerWagnerMinCut(cycle)
              << " (expected 2)\n";

    return 0;
}