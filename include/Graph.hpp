#pragma once

#include <vector>

class Graph {
public:
    explicit Graph(int vertices);

    void addEdge(int u, int v, int weight);
    int getWeight(int u, int v) const;
    int vertexCount() const;
    bool isActive(int vertex) const;
    void mergeVertices(int s, int t);

private:
    int vertices_;
    std::vector<std::vector<int>> weights_;
    std::vector<bool> active_;
};