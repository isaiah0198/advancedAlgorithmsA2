#include <iostream>
#include "Graph.hpp"
#include "StoerWagner.hpp"
#include "Karger.hpp"
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <set>

long long median(std::vector<long long> values) {
    std::sort(values.begin(), values.end());
    return values[values.size() / 2];
}

Graph makeRandomConnectedGraph(
    int vertices,
    int extraEdges,
    unsigned int seed
) {
    Graph graph(vertices);
    std::mt19937 rng(seed);

    for (int v = 1; v < vertices; ++v) {
        std::uniform_int_distribution<int> parentDist(0, v - 1);
        int parent = parentDist(rng);

        graph.addEdge(parent, v, 1);
    }

    std::set<std::pair<int, int>> edges;

    for (int v = 1; v < vertices; ++v) {
        for (int u = 0; u < v; ++u) {
            if (graph.getWeight(u, v) > 0) {
                edges.insert({u, v});
            }
        }
    }

    std::uniform_int_distribution<int> vertexDist(0, vertices - 1);

    while (extraEdges > 0) {
        int u = vertexDist(rng);
        int v = vertexDist(rng);

        if (u == v) {
            continue;
        }

        if (u > v) {
            std::swap(u, v);
        }

        if (edges.count({u, v}) > 0) {
            continue;
        }

        graph.addEdge(u, v, 1);
        edges.insert({u, v});

        --extraEdges;
    }

    return graph;
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

    Graph random10 = makeRandomConnectedGraph(10, 10, 42);

        std::cout << "\nRandom 10-node graph:\n";

        std::cout << "Stoer-Wagner: "
                  << stoerWagnerMinCut(random10)
                  << '\n';

        std::cout << "Karger repeated: "
                  << kargerRepeatedMinCut(random10, 50, 0)
                  << '\n';

        std::cout << "\nSize scaling test:\n";

int sizes[] = {10, 25, 50, 100};

for (int vertices : sizes) {
    Graph graph = makeRandomConnectedGraph(
        vertices,
        vertices,
        42
    );

    int exactCut = stoerWagnerMinCut(graph);

    std::vector<long long> swTimes;
    std::vector<long long> kargerTimes;

    for (int run = 0; run < 51; ++run) {
        auto swStart =
            std::chrono::high_resolution_clock::now();

        int swResult =
            stoerWagnerMinCut(graph);

        auto swEnd =
            std::chrono::high_resolution_clock::now();

        swTimes.push_back(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                swEnd - swStart
            ).count()
        );

        auto kStart =
            std::chrono::high_resolution_clock::now();

        int kargerResult =
            kargerRepeatedMinCut(
                graph,
                20,
                run * 1000
            );

        auto kEnd =
            std::chrono::high_resolution_clock::now();

        kargerTimes.push_back(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                kEnd - kStart
            ).count()
        );
    }

    std::cout << "Vertices: "
              << vertices
              << ", exact cut: "
              << exactCut
              << ", Stoer-Wagner median: "
              << median(swTimes)
              << " ns, Karger x20 median: "
              << median(kargerTimes)
              << " ns\n";
}

    std::cout << "\nDensity test:\n";

    int densityVertices = 50;

    double densities[] = {
        0.10,
        0.25,
        0.50,
        0.75
    };

    int maxEdges =
        densityVertices *
        (densityVertices - 1) / 2;

    for (double density : densities) {

        int targetEdges =
            static_cast<int>(
                density * maxEdges
            );

        int extraEdges =
            targetEdges -
            (densityVertices - 1);

        Graph graph =
            makeRandomConnectedGraph(
                densityVertices,
                extraEdges,
                42
            );

        int exactCut =
            stoerWagnerMinCut(graph);


        // Single-run Karger reliability
        int singleRunSuccesses = 0;
        int densityExperiments = 100;

        for (int experiment = 0;
            experiment < densityExperiments;
            ++experiment) {

            int result =
                kargerMinCut(
                    graph,
                    experiment * 1000
                );

            if (result == exactCut) {
                ++singleRunSuccesses;
            }
        }


        // Runtime measurements
        std::vector<long long> swTimes;
        std::vector<long long> kargerTimes;

        for (int run = 0; run < 51; ++run) {

            auto swStart =
                std::chrono::high_resolution_clock::now();

            int swResult =
                stoerWagnerMinCut(graph);

            auto swEnd =
                std::chrono::high_resolution_clock::now();

            swTimes.push_back(
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    swEnd - swStart
                ).count()
            );


            auto kStart =
                std::chrono::high_resolution_clock::now();

            int kargerResult =
                kargerRepeatedMinCut(
                    graph,
                    20,
                    run * 1000
                );

            auto kEnd =
                std::chrono::high_resolution_clock::now();

            kargerTimes.push_back(
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    kEnd - kStart
                ).count()
            );
        }


        std::cout << "Density: "
                << density
                << ", edges: "
                << targetEdges
                << ", exact cut: "
                << exactCut
                << ", single Karger success: "
                << singleRunSuccesses
                << " / "
                << densityExperiments
                << ", SW median: "
                << median(swTimes)
                << " ns"
                << ", Karger x20 median: "
                << median(kargerTimes)
                << " ns\n";
    }

            unsigned int graphSeeds[] = {
            42,
            142,
            242,
            342,
            442
        };

        std::cout << "\nDensity reliability across graph seeds:\n";

        for (double density : densities) {
            int totalSuccesses = 0;
            int totalTrials = 0;

            int targetEdges =
                static_cast<int>(density * maxEdges);

            int extraEdges =
                targetEdges - (densityVertices - 1);

            for (unsigned int graphSeed : graphSeeds) {
                Graph graph = makeRandomConnectedGraph(
                    densityVertices,
                    extraEdges,
                    graphSeed
                );

                int exactCut = stoerWagnerMinCut(graph);

                for (int experiment = 0;
                    experiment < 100;
                    ++experiment) {

                    int result = kargerMinCut(
                        graph,
                        experiment * 1000
                    );

                    if (result == exactCut) {
                        ++totalSuccesses;
                    }

                    ++totalTrials;
                }
            }

            std::cout << "Density: "
                    << density
                    << ", successes: "
                    << totalSuccesses
                    << " / "
                    << totalTrials
                    << '\n';
        }

    
    
        std::cout << "\nKarger repetitions across densities:\n";

        int densityRepetitionCounts[] = {1, 10, 20};
        int trialsPerGraph = 50;

        for (double density : densities) {

            int targetEdges =
                static_cast<int>(density * maxEdges);

            int extraEdges =
                targetEdges - (densityVertices - 1);

            for (int repetitions : densityRepetitionCounts) {

                int totalSuccesses = 0;
                int totalTrials = 0;

                for (unsigned int graphSeed : graphSeeds) {

                    Graph graph = makeRandomConnectedGraph(
                        densityVertices,
                        extraEdges,
                        graphSeed
                    );

                    int exactCut =
                        stoerWagnerMinCut(graph);

                    for (int experiment = 0;
                        experiment < trialsPerGraph;
                        ++experiment) {

                        unsigned int baseSeed =
                            graphSeed * 100000
                            + experiment * 1000;

                        int result =
                            kargerRepeatedMinCut(
                                graph,
                                repetitions,
                                baseSeed
                            );

                        if (result == exactCut) {
                            ++totalSuccesses;
                        }

                        ++totalTrials;
                    }
                }

                std::cout << "Density: "
                        << density
                        << ", repetitions: "
                        << repetitions
                        << ", successes: "
                        << totalSuccesses
                        << " / "
                        << totalTrials
                        << '\n';
            }
        }

    return 0;
}
