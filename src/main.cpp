#include <iostream>
#include "Graph.hpp"
#include "StoerWagner.hpp"
#include "Karger.hpp"

int main() {
    Graph triangle(3);
    triangle.addEdge(0, 1, 3);
    triangle.addEdge(0, 2, 2);
    triangle.addEdge(1, 2, 4);

    Graph path(4);
    path.addEdge(0, 1, 1);
    path.addEdge(1, 2, 1);
    path.addEdge(2, 3, 1);

    Graph cycle(4);
    cycle.addEdge(0, 1, 1);
    cycle.addEdge(1, 2, 1);
    cycle.addEdge(2, 3, 1);
    cycle.addEdge(3, 0, 1);

    Graph fiveNode(5);
    fiveNode.addEdge(0, 1, 3);
    fiveNode.addEdge(0, 2, 2);
    fiveNode.addEdge(1, 2, 1);
    fiveNode.addEdge(1, 3, 4);
    fiveNode.addEdge(1, 4, 1);
    fiveNode.addEdge(2, 3, 2);
    fiveNode.addEdge(2, 4, 3);
    fiveNode.addEdge(3, 4, 2);

    std::cout << "Triangle:\n";
    std::cout << "Stoer-Wagner: "
              << stoerWagnerMinCut(triangle)
              << '\n';

    std::cout << "Karger repeated: "
              << kargerRepeatedMinCut(triangle, 20, 0)
              << "\n\n";

    std::cout << "Path:\n";
    std::cout << "Stoer-Wagner: "
              << stoerWagnerMinCut(path)
              << '\n';

    std::cout << "Karger repeated: "
              << kargerRepeatedMinCut(path, 20, 0)
              << "\n\n";

    std::cout << "Cycle:\n";
    std::cout << "Stoer-Wagner: "
              << stoerWagnerMinCut(cycle)
              << '\n';

    std::cout << "Karger repeated: "
              << kargerRepeatedMinCut(cycle, 20, 0)
              << '\n';

    std::cout << "\nFive-node graph:\n";

    std::cout << "Stoer-Wagner: "
              << stoerWagnerMinCut(fiveNode)
              << '\n';

    std::cout << "Karger repeated: "
              << kargerRepeatedMinCut(fiveNode, 50, 0)
              << '\n';

    return 0;
}