#include "floyd.h"
#include "random_int.h"

#include <cassert>
#include <cmath>
#include <thread>
#include <iomanip>
#include <vector>
#include <algorithm>

// Generate random 2D vertices within a square of given side length
void generate_vertices(std::size_t n, Vertices& points, int side) {
    RandomInt<> ranInt(0, side);
    for (std::size_t i = 0; i < n; ++i) {
        points.emplace_back(ranInt(), ranInt());
    }
}

// Generate an adjacency matrix with distances between nearby vertices
void generate_edges(double distance, Vertices const& points, Matrix& M) {
    int n = points.size();
    M.resize(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double d = std::hypot(
                points[i].first - points[j].first,
                points[i].second - points[j].second
            );

            if (d <= distance) {
                M.set(i, j, std::round(d));
            } else {
                M.set(i, j, 1'000'000); // Use a large constant to represent "infinity"
            }
        }
    }
}

// Standard Floyd-Warshall algorithm for all-pairs shortest paths
void minimum_distance(Matrix& M) {
    int n = M.size();
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                M.set(i, j, std::min(M.get(i, j), M.get(i, k) + M.get(k, j)));
}

// Parallelized Floyd-Warshall algorithm
void minimum_distance_par(Matrix& M) {
    int n = M.size();
    int num_threads = std::thread::hardware_concurrency();
    std::vector<std::jthread> threads(num_threads);
    std::vector<int> borders;

    for (int i = 0; i < num_threads; ++i) {
        borders.push_back(i * (n / num_threads));
        if (i == num_threads - 1) {
            borders.push_back(n);
        }
    }

    auto compute = [&](int rank, int k) {
        for (int i = borders[rank]; i < borders[rank + 1]; ++i) {
            for (int j = 0; j < n; ++j) {
                M.set(i, j, std::min(M.get(i, j), M.get(i, k) + M.get(k, j)));
            }
        }
    };

    for (int k = 0; k < n; ++k) {
        for (int j = 0; j < num_threads; ++j) {
            threads[j] = std::jthread(compute, j, k);
        }

        for (int j = 0; j < num_threads; ++j) {
            threads[j].join();
        }
    }
}

// Nicely formatted matrix output
void print(std::ostream& out, Matrix const& M) {
    const int width = 10;

    for (int i = 0; i < M.size(); ++i) {
        for (int j = 0; j < M.size(); ++j) {
            if (M.get(i, j) < 1'000'000) {
                out << std::setw(width) << M.get(i, j);
            } else {
                out << std::setw(width) << "inf";
            }
        }
        out << '\n';
    }
}
