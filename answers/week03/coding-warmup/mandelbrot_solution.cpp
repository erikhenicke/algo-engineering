#include <complex>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <vector>
#include "coding_warmup.h"

using namespace std;

constexpr int width = 4000;
constexpr int height = width;

void compute_mandelbrot_image_solution() { // generate mandelbrot pgm (portable graymap)
  const string image_name = "mandelbrot.pgm";
  remove(image_name.c_str()); // remove file from disk
  const double start = omp_get_wtime();
  vector<string> look_up{256}; // look-up table for strings
  for (int i = 0; i < 256; ++i) { look_up[i] = to_string(i) + "\n"; } // fill look-up table
  ofstream image(image_name); // file output stream
  if (image.is_open()) {
    image << "P2\n" << width << " " << height << " 255\n"; // write pgm header
#pragma omp parallel // start parallel region
    {
      string buffer; // each thread has it's own string buffer
      buffer.reserve(width * 4); // reserve enough space to avoid reallocation
#pragma omp for schedule(dynamic) ordered // we could also use schedule(static, 1)
      for (int i = 0; i < height; i++) {
        buffer.clear(); // clear content of buffer
        for (int j = 0; j < width; j++) {
          buffer += look_up[compute_pixel(j, i)]; // fill buffer
        }
#pragma omp ordered // write rows of image in sequential order
        image << buffer; // write string buffer to file
      }
    } // end parallel region
    image.close(); // close file output stream
  } else {
    cout << "Could not open the file!";
  }
  cout << omp_get_wtime() - start << " seconds" << endl;
}
