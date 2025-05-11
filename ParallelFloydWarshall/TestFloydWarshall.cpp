#include "floyd.h"
#include "clock.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cassert>

// Usage: main [n] [distance] [side]
// n        = number of generated cities
// distance = maximum distance for connecting two cities
// side     = side length of the square within which cities are generated
int main(int argc, char* argv[]) {
    // Command-line parameters
    int noPts = 6;
    if (argc > 1)
        noPts = std::stoi(argv[1]);

    double distance = 40;
    if (argc > 2)
        distance = std::stod(argv[2]);

    int side = 100;
    if (argc > 3)
        side = std::stoi(argv[3]);

    assert(distance < static_cast<double>(side));

    // Generate adjacency matrix
    Vertices points;
    generate_vertices(noPts, points, side);
    Matrix M;
    generate_edges(distance, points, M);

    std::cout << "Initial matrix:\n";

    // Save the initial matrix to file
    std::ofstream out("mat_init.txt");
    if (!out)
        throw std::runtime_error("Cannot open mat_init.txt for writing.");
    print(out, M);
    out.close();

    // Copy the matrix before modification by the sequential algorithm
    Matrix M1 = M;

    // Sequential Floyd-Warshall
    Clock clock;
    minimum_distance(M);
    auto [time, unit] = clock.stop(Clock::microsec);
    std::cout << "Sequential time = " << time << unit << "\n";

    out.open("mat_seq.txt");
    if (!out)
        throw std::runtime_error("Cannot open mat_seq.txt for writing.");
    print(out, M);
    out.close();

    // Parallel Floyd-Warshall
    clock.start();
    minimum_distance_par(M1);
    auto [time1, unit1] = clock.stop(Clock::microsec);
    std::cout << "Parallel time = " << time1 << unit1 << "\n";

    out.open("mat_par.txt");
    if (!out)
        throw std::runtime_error("Cannot open mat_par.txt for writing.");
    print(out, M1);
    out.close();

    return 0;
}
