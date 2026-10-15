#include "StoerWagner.hpp"
#include <vector>
#include <iostream>

int stoerWagnerMinCut(Graph graph) {
    int bestCut = 0;

    while (graph.activeVertexCount() > 1) {
        int prev = -1;

        std::vector<int> connectionWeight(graph.vertexCount(), 0);
        std::vector<bool> added(graph.vertexCount(), false);

        for (int i = 0; i < graph.activeVertexCount(); ++i) {
            int selected = -1;
            
            for (int v = 0; v < graph.vertexCount(); ++v) {
                if (!graph.isActive(v) || added[v]) {
                    continue;
                }

                if (selected == -1 || connectionWeight[v] > connectionWeight[selected]) {
                    selected = v;
                }
            }

            std::cout << "i = " << i << ", selected = " << selected << ", active = " << graph.activeVertexCount() << '\n';

            added[selected] = true;
            
            for (int v = 0; v < graph.vertexCount(); ++v) {
                if (!graph.isActive(v) || added[v]) {
                    continue;
                }

                connectionWeight[v] += graph.getWeight(selected, v);
            }
            prev = selected;
        }
        
        break;
    }

    return bestCut;
}