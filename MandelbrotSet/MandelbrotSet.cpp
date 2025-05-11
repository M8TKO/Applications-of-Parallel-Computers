#include <pngwriter.h>
#include <cmath>
#include <thread>
#include <iostream>
#include <complex>

using namespace std::literals;
using std::this_thread::sleep_for;

// Check if a complex number is in the Mandelbrot set
bool isInMandelbrotSet(std::complex<double> c) {
    std::complex<double> z = 0;
    for (int i = 0; i < 1000; ++i) {
        z = z * z + c;
        if (std::abs(z) > 2)
            return false;
    }
    return true;
}

// Render a quadrant of the Mandelbrot set
void mandelbrot(pngwriter& png, double x_start, double x_end, double y_start, double y_end, int rank) {
    int startPixel_X, startPixel_Y;
    double startValue_X, startValue_Y, endValue_X, endValue_Y;

    switch (rank) {
        case 0:
            startPixel_X = startPixel_Y = 0;
            startValue_X = x_start;
            startValue_Y = y_start;
            endValue_X = (x_start + x_end) / 2;
            endValue_Y = (y_start + y_end) / 2;
            break;
        case 1:
            startPixel_X = 400;
            startPixel_Y = 0;
            startValue_X = (x_start + x_end) / 2;
            startValue_Y = y_start;
            endValue_X = x_end;
            endValue_Y = (y_start + y_end) / 2;
            break;
        case 2:
            startPixel_X = 0;
            startPixel_Y = 400;
            startValue_X = x_start;
            startValue_Y = (y_start + y_end) / 2;
            endValue_X = (x_start + x_end) / 2;
            endValue_Y = y_end;
            break;
        case 3:
            startPixel_X = 400;
            startPixel_Y = 400;
            startValue_X = (x_start + x_end) / 2;
            startValue_Y = (y_start + y_end) / 2;
            endValue_X = x_end;
            endValue_Y = y_end;
            break;
    }

    for (int q = startPixel_Y; q < startPixel_Y + 400; ++q) {
        double y_value = startValue_Y + static_cast<double>(q - startPixel_Y) * (endValue_Y - startValue_Y) / 399.0;

        for (int p = startPixel_X; p < startPixel_X + 400; ++p) {
            double x_value = startValue_X + static_cast<double>(p - startPixel_X) * (endValue_X - startValue_X) / 399.0;

            std::complex<double> c(x_value, y_value);
            if (isInMandelbrotSet(c)) {
                png.plot(p, q, 0, 0, 0);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <xmin> <xmax> <ymin> <ymax> <output_file>" << std::endl;
        return 1;
    }

    double startX = std::stod(argv[1]);
    double endX = std::stod(argv[2]);
    double startY = std::stod(argv[3]);
    double endY = std::stod(argv[4]);
    std::string outputFile = argv[5];

    while (true) {
        pngwriter png(800, 800, 1.0, outputFile.c_str());

        std::thread threads[4];
        for (int i = 0; i < 4; ++i) {
            threads[i] = std::thread(mandelbrot, std::ref(png), startX, endX, startY, endY, i);
        }

        for (int i = 0; i < 4; ++i) {
            threads[i].join();
        }

        png.close();

        std::cout << "Image created and saved as " << outputFile << std::endl;

        std::cout << "Would you like to generate a new image? (y/n): ";
        char choice;
        std::cin >> choice;

        if (choice != 'y' && choice != 'Y') {
            break;
        }

        std::cout << "Enter new domain (xmin xmax ymin ymax): ";
        std::cin >> startX >> endX >> startY >> endY;

        std::cout << "Enter new output PNG filename (including .png): ";
        std::cin >> outputFile;
    }

    return 0;
}
