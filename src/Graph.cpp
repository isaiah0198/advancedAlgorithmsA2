#include "Graph.hpp"

Graph::Graph(int vertices)
    : vertices_(vertices), weights_(vertices, std::vector<int>(vertices, 0)) {}

void Graph::addEdge(int u, int v, int weight) {
    weights_[u][v] = weight;
}

int Graph::getWeight(int u, int v) const {
    return weights_[u][v];
}