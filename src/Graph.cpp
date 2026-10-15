#include "Graph.hpp"

Graph::Graph(int vertices)
    :   vertices_(vertices), 
        weights_(vertices, std::vector<int>(vertices, 0)), 
        active_(vertices, true) {}

void Graph::addEdge(int u, int v, int weight) {
    weights_[u][v] = weight;
    weights_[v][u] = weight; // For an undirected graph
}

int Graph::getWeight(int u, int v) const {
    return weights_[u][v];
}

int Graph::vertexCount() const {
    return vertices_;
}

bool Graph::isActive(int vertex) const {
    return active_[vertex];
}

void Graph::mergeVertices(int s, int t) {
    for (int v = 0; v < vertices_; ++v) {
        if (v == s || v == t || !active_[v]) {
            continue;
        }

        weights_[s][v] += weights_[t][v];
        weights_[v][s] += weights_[s][v];
    }

    active_[t] = false;
}