#include "Karger.hpp"
#include <random>
#include <utility>
#include <iostream>
#include <limits>

static std::pair<int, int> chooseRandomEdge(
    const Graph& graph,
    std::mt19937& rng
) {
    int totalWeight = 0;

    for (int u = 0; u < graph.vertexCount(); ++u) {
        if (!graph.isActive(u)) {
            continue;
        }

        for (int v = u + 1; v < graph.vertexCount(); ++v) {
            if (!graph.isActive(v)) {
                continue;
            }

            totalWeight += graph.getWeight(u, v);
        }
    }

    std::uniform_int_distribution<int> dist(1, totalWeight);
    int target = dist(rng);

    int runningWeight = 0;

    for (int u = 0; u < graph.vertexCount(); ++u) {
        if (!graph.isActive(u)) {
            continue;
        }

        for (int v = u + 1; v < graph.vertexCount(); ++v) {
            if (!graph.isActive(v)) {
                continue;
            }

            runningWeight += graph.getWeight(u, v);

            if (runningWeight >= target) {
                return {u, v};
            }
        }
    }

    return {-1, -1};
}

int kargerMinCut(Graph graph, unsigned int seed) {
    std::mt19937 rng(seed);

    while (graph.activeVertexCount() > 2) {
        auto [u, v] = chooseRandomEdge(graph, rng);
        graph.mergeVertices(u, v);
    }

    for (int u = 0; u < graph.vertexCount(); ++u) {
        if (!graph.isActive(u)) {
            continue;
        }

        for (int v = u + 1; v < graph.vertexCount(); ++v) {
            if (!graph.isActive(v)) {
                continue;
            }

            return graph.getWeight(u, v);
        }
    }

    return 0;
}

int kargerRepeatedMinCut(
    const Graph& graph,
    int repetitions,
    unsigned int baseSeed
) {
    int bestCut = std::numeric_limits<int>::max();

    for (int i = 0; i < repetitions; ++i) {
        int cut = kargerMinCut(
            graph,
            baseSeed + i
        );

        if (cut < bestCut) {
            bestCut = cut;
        }
    }

    return bestCut;
}