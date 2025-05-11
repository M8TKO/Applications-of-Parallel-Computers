#pragma once
#include <cstddef>
#include <vector>
#include <iostream>
#include <tuple>

using Vertices = std::vector<std::pair<int, int>>;

class Matrix {
    std::vector<std::vector<int>> data;
public:
    void resize(int n) {
        data.resize(n);
        for (int i = 0; i < n; ++i)
            data[i].resize(n);
    }

    void set(int i, int j, int x) {
        data[i][j] = x;
    }

    int size() const {
        return data.size();
    }

    int get(int i, int j) const {
        return data[i][j];
    }
};

void generate_vertices(std::size_t n, Vertices& points, int side = 1000);
void generate_edges(double distance, Vertices const& points, Matrix& M);
void minimum_distance(Matrix& M);
void minimum_distance_par(Matrix& M);
void print(std::ostream& out, Matrix const& M);
