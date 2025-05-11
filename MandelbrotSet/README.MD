# Mandelbrot Set Renderer (C++ with PNG Output)

This project generates visualizations of the Mandelbrot set using C++ and outputs the result as a `.png` image using the [`pngwriter`](http://pngwriter.sourceforge.net/) library. It employs multithreading to compute different quadrants of the image in parallel, resulting in efficient rendering.

## 🌀 Mandelbrot Set

The Mandelbrot set is a famous fractal named after mathematician Benoît Mandelbrot. It is defined by iterating the complex quadratic polynomial:

```
zₙ₊₁ = zₙ² + c
```

A point `c` in the complex plane belongs to the Mandelbrot set if this sequence does not diverge when iterated from `z₀ = 0`.

## 🛠️ Features

- Multithreaded rendering (4 threads for 4 image quadrants)
- Adjustable viewing window (domain in the complex plane)
- Interactive CLI: supports repeated image generation with new parameters
- Black-and-white visualization (black pixels represent points in the set)

## 🧱 Dependencies

- C++17 or newer
- [pngwriter](http://pngwriter.sourceforge.net/) library
- libpng (usually installed as a dependency of pngwriter)

## 🧪 Compilation

To compile the program, link it with the `pngwriter` and `png` libraries. For example:

```bash
g++ -std=c++17 -lpngwriter -lpng -lz -o mandelbrot mandelbrot.cpp
```

> Make sure `pngwriter` and its dependencies are installed and accessible via your system's compiler and linker paths.

## 🚀 Usage

```bash
./mandelbrot <xmin> <xmax> <ymin> <ymax> <output_file.png>
```

### Example:

```bash
./mandelbrot -2 1 -1.5 1.5 output.png
```

This will generate a Mandelbrot image from the domain `[-2, 1]` × `[-1.5, 1.5]` and save it as `output.png`.

After rendering, the program will ask if you want to generate a new image with different parameters.

## 📸 Sample Output

![mandelbrot_sample](preview.png)

> Replace this with your actual image or remove if not needed.

## 💡 Notes

- The image size is fixed at **800×800 pixels**, with each thread computing one 400×400 quadrant.
- Pixel color is black for points inside the Mandelbrot set, and white for others (background).
- The core Mandelbrot iteration uses a maximum of 1000 iterations for divergence checking.

## ✅ Status

This project is complete and functions as expected. No additional runtime testing is necessary.

## 📜 License

MIT License (or specify yours)

## 🤝 Acknowledgments

- Inspired by classical visualizations of the Mandelbrot set
- Built with [`pngwriter`](http://pngwriter.sourceforge.net/)
