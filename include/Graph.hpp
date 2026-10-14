#pragma once

#include <vector>

class Graph {
public:
    explicit Graph(int vertices);

    void addEdge(int u, int v, int weight);
    int getWeight(int u, int v) const;
    int vertexCount() const;

private:
    int vertices_;
    std::vector<std::vector<int>> weights_;
};