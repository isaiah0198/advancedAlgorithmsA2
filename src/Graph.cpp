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