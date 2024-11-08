//
//Created by erik on 03.11.24.
//

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "exam_exercises_c.h"

double rnd(unsigned int* seed) {
  *seed = (1140671485 * (*seed) + 12820163) % (1 << 24);
  return ((double)*seed) / (1 << 24);
}

int neighbors_sp(unsigned char *plane, int size, int x, int y) {
  if (x < 0 || y < 0 || x >= size || y >= size) {
    exit(1);
  }
  int count = 0;
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {

      if (dx == 0 && dy == 0) {
        continue;
      }

      int x_dash = x + dx;
      int y_dash = y + dy;

      if (x_dash >= 0 && x_dash < size && y_dash >= 0 && y_dash < size) {
        count += *(plane + (x_dash * size + y_dash));
      }
    }
  }
  return count;
}

int neighbors_dp(char **plane, int size, int x, int y) {
  if (x < 0 || y < 0 || x >= size || y >= size) {
    exit(1);
  }
  int count = 0;
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {

      if (dx == 0 && dy == 0) {
        continue;
      }

      int x_dash = x + dx;
      int y_dash = y + dy;

      if (x_dash >= 0 && x_dash < size && y_dash >= 0 && y_dash < size) {
        count += plane[x_dash][y_dash];
      }
    }
  }
  return count;
}

void write_to_image_sp(const char *filename, unsigned char *image,
                    const int size) {

#pragma omp parallel for collapse(2)
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      *(image + i * size + j) = -255 * ((int) *(image + i * size + j) - 1);
    }
  }

  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Could not open the file!");
    exit(1);
  }

  fprintf(file, "P5\n%d %d 255\n", size, size); // header
  fwrite(image, sizeof(unsigned char), size * size, file);

  fclose(file);
}

void game_of_life_sp(int size, int gens, int num_threads, bool outputImages) {

  unsigned int seed = 0;
  unsigned char* plane = malloc(size * size * sizeof(unsigned char));
  unsigned char* aux_plane = malloc(size * size * sizeof(unsigned char));
  int total_gens = gens;
  unsigned char *image = malloc(size * size * sizeof(unsigned char));

#pragma omp parallel
  {
    unsigned int thread_seed = seed + omp_get_thread_num();
#pragma for collapse(2) schedule(static, 1)
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        *(plane + (i * size + j)) = rnd(&thread_seed) >= 0.5;
      }
    }
  }

  char output_name[50];
  if (outputImages) {
    memcpy(image, plane, size * size * sizeof(unsigned char));
    sprintf(output_name, "output%d.ppm", total_gens - gens);
    write_to_image_sp(output_name, image, size);
  }

  const double start = omp_get_wtime();

  while(gens-- > 0) {
    #pragma omp parallel for num_threads(num_threads) schedule(static, size / omp_get_num_threads())
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          const int neighs = neighbors_sp(plane, size, i, j);
          switch (*(plane + (i * size + j))) {
            case 0: *(aux_plane + (i * size + j)) = (neighs == 3);
            break;
            case 1: *(aux_plane + (i * size + j)) = (neighs == 2) || (neighs == 3);
            break;
            default:;
          }
        }
     }
    unsigned char *tmp_plane = aux_plane;
    aux_plane = plane;
    plane = tmp_plane;
    if (outputImages) {
      memcpy(image, plane, size * size * sizeof(unsigned char));
      sprintf(output_name, "output%d.ppm", total_gens - gens);
      write_to_image_sp(output_name, image, size);
    }
  }

  printf("Game of life with single pointers, size = %d, gens = %d, "
         "threads = %d in %f seconds\n", size, total_gens, num_threads,
         omp_get_wtime() - start);

  free(aux_plane);
  free(plane);
  // if (outputImages) {
  //   free(image);
  // }
}

char **allocate_plane(int size) {
  char **plane = malloc(size * sizeof(char *)); // sizeof(char*) equals 8 already
  for (int i = 0; i < size; i++) {
    plane[i] = malloc(size * sizeof(char)); // one cache line per element
  }
  return plane;
}

void free_plane(char **plane, int size) {
  for (int i = 0; i < size; i++) {
    free(plane[i]);
  }
  free(plane);
}

// game of life
void game_of_life_dp(int size, int gens, int num_threads) {
  int total_gens = gens;
  unsigned int seed = time(NULL);
  char **plane = allocate_plane(size);
  char **aux_plane = allocate_plane(size);

#pragma omp parallel
  {
    unsigned int thread_seed = seed + omp_get_thread_num();
#pragma for collapse(2) schedule(static, 1)
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        plane[i][j] = rnd(&thread_seed) >= 0.5;
      }
    }
  }

  const double start = omp_get_wtime();

  while(gens-- > 0) {
#pragma omp parallel for num_threads(num_threads) schedule(static, size / omp_get_num_threads())
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        const int neighs = neighbors_dp(plane, size, i, j);
        switch (plane[i][j]) {
          case 0: aux_plane[i][j] = (neighs == 3);
          break;
          case 1: aux_plane[i][j] = (neighs == 2) || (neighs == 3);
          break;
          default:;
        }
      }
    }
    char **tmp_plane = aux_plane;
    aux_plane = plane;
    plane = tmp_plane;
  }

  printf("Game of life with double pointers, size = %d, gens = %d, "
         "threads = %d in %f seconds\n", size, total_gens, num_threads,
         omp_get_wtime() - start);

  free_plane(plane, size);
  free_plane(aux_plane, size);
}
