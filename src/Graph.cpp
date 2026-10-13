#include "Graph.hpp"

Graph::Graph(int vertices)
    : vertices_(vertices), weights_(vertices, std::vector<int>(vertices, 0)) {}