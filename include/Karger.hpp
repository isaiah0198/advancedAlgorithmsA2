#pragma once

#include "Graph.hpp"

int kargerMinCut(Graph graph, unsigned int seed);

int kargerRepeatedMinCut(
    const Graph& graph,
    int repetitions,
    unsigned int baseSeed
);