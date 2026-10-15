#include "StoerWagner.hpp"
#include <vector>
#include <iostream>
#include <limits>
#include <algorithm>

int stoerWagnerMinCut(Graph graph) {
    int bestCut = std::numeric_limits<int>::max();

    while (graph.activeVertexCount() > 1) {
        int prev = -1;

        std::vector<int> connectionWeight(graph.vertexCount(), 0);
        std::vector<bool> added(graph.vertexCount(), false);

        int phaseSize = graph.activeVertexCount();

        for (int i = 0; i < phaseSize; ++i) {
            int selected = -1;
            
            for (int v = 0; v < graph.vertexCount(); ++v) {
                if (!graph.isActive(v) || added[v]) {
                    continue;
                }

                if (selected == -1 || connectionWeight[v] > connectionWeight[selected]) {
                    selected = v;
                }
            }

            added[selected] = true;

            if (i == phaseSize - 1) {
                                
                bestCut = std::min(bestCut, connectionWeight[selected]);
                graph.mergeVertices(prev, selected);
            }

            for (int v = 0; v < graph.vertexCount(); ++v) {
                if (!graph.isActive(v) || added[v]) {
                    continue;
                }

                connectionWeight[v] += graph.getWeight(selected, v);
            }
            prev = selected;
        }
    }

    return bestCut;
}