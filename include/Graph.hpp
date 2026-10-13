#pragma once

#include <vector>

class Graph {
public:
    explicit Graph(int vertices);

private:
    int vertices_;
    std::vector<std::vector<int>> weights_;
};