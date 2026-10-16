#include <iostream>
#include "Graph.hpp"
#include "StoerWagner.hpp"
#include "Karger.hpp"
#include <chrono>
#include <vector>
#include <algorithm>

long long median(std::vector<long long> values) {
    std::sort(values.begin(), values.end());
    return values[values.size() / 2];
}

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

    int exactCut = stoerWagnerMinCut(fiveNode);

    int repetitionCounts[] = {1, 2, 5, 10, 20, 50};
    int experiments = 100;

    for (int repetitions : repetitionCounts) {
        int successes = 0;

        for (int experiment = 0; experiment < experiments; ++experiment) {
            unsigned int baseSeed = experiment * 1000;

            int result = kargerRepeatedMinCut(
                fiveNode,
                repetitions,
                baseSeed
            );

            if (result == exactCut) {
                ++successes;
            }
        }

        std::cout << "Repetitions: " << repetitions
                  << ", successes: "
                  << successes << " / "
                  << experiments << '\n';
    }

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

    std::cout << "\nTiming test:\n";

    std::vector<long long> swTimes;

    for (int run = 0; run < 51; ++run) {
        auto start = std::chrono::high_resolution_clock::now();

        int result = stoerWagnerMinCut(fiveNode);

        auto end = std::chrono::high_resolution_clock::now();

        swTimes.push_back(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                end - start
            ).count()
        );
    }

    std::cout << "Stoer-Wagner median: "
            << median(swTimes)
            << " ns\n";

    for (int repetitions : repetitionCounts) {
        std::vector<long long> kargerTimes;

        for (int run = 0; run < 51; ++run) {
            unsigned int seed = run * 1000;

            auto start = std::chrono::high_resolution_clock::now();

            int result = kargerRepeatedMinCut(
                fiveNode,
                repetitions,
                seed
            );

            auto end = std::chrono::high_resolution_clock::now();

            kargerTimes.push_back(
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    end - start
                ).count()
            );
        }

        std::cout << "Karger repetitions "
                << repetitions
                << " median: "
                << median(kargerTimes)
                << " ns\n";
    }

    return 0;
}