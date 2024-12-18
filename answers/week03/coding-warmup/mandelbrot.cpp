#include <omp.h>
#include <complex>
#include <fstream>
#include <iostream>
#include <vector>
#include "coding_warmup.h"

using namespace std;

constexpr int width = 4000;
constexpr int height = width;

int compute_pixel(int x, int y) {  // compute the gray value of a pixel
  complex<double> point(2.0 * x / width - 1.5, 2.0 * y / height - 1.0);
  complex<double> z(0, 0);
  constexpr int iterations = 100;
  int nb_iter = 0;
  // test if point in mandelbrot set
  while (abs(z) < 2 && nb_iter < iterations) {
    z = z * z + point;
    nb_iter++;
  }
  // white color (255) if point in mandelbrot set, else a shade of gray
  return (255 * nb_iter) / iterations;
}

void compute_mandelbrot_image() {  // generate mandelbrot pgm (portable graymap)

  vector<string> lookup(256);
#pragma omp parallel for
  for (int i = 0; i < 256; i++) {
    lookup[i] = to_string(i) + "\n";
  }

  const double start = omp_get_wtime();
    vector<vector<int>> pixels(height, vector<int>(width, 0));
#pragma omp parallel for collapse(2) schedule(dynamic, 1000)
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        pixels[i][j] = compute_pixel(i, j);
      }
    }

  const string image_name = "mandelbrot.pgm";
  remove(image_name.c_str());  // remove file from disk
  ofstream image(image_name);  // file output stream
  if (image.is_open()) {
    image << "P2\n" << width << " " << height << " 255\n";  // pgm header
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        image << lookup[pixels[j][i]];  // write pixel value
      }
    }
    image.close();  // close file output stream
  } else {
    cout << "Could not open the file!";
  }
  cout << omp_get_wtime() - start << " seconds" << endl;
}
