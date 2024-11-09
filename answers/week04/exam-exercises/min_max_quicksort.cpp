#include <omp.h>
#include <iostream>
#include <limits>
#include <parallel/algorithm>
#include <vector>
#include "exam_exercises.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

// compute the average of two integers without overflow
inline int64_t average(int64_t a, int64_t b) {
  return (a & b) + ((a ^ b) >> 1);
}

// partitioning function for quicksort
inline int64_t partition(int64_t* arr, int64_t left, int64_t right,
                         int64_t pivot, int64_t& smallest, int64_t& biggest) {
  int64_t* left_ptr = &arr[left];
  int64_t* right_ptr = &arr[right];
  while (left_ptr < right_ptr) {
    smallest = (*left_ptr < smallest) ? *left_ptr : smallest;
    biggest = (*left_ptr > biggest) ? *left_ptr : biggest;
    if (*left_ptr > pivot) {
      --right_ptr;
      std::swap(*left_ptr, *right_ptr);
    } else {
      ++left_ptr;
    }
  }
  return left_ptr - arr;
}

inline void insertion_sort(int64_t* arr, int64_t left, int64_t right) {
  for (int64_t i = left + 1; i <= right; i++) {
    int64_t key = arr[i];
    int64_t j = i - 1;
    while (j >= left && arr[j] > key) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

// the core recursive quicksort function
void qs_core(int64_t* arr, int64_t left, int64_t right, const int64_t pivot) {
  if (right - left < 32) {
    insertion_sort(arr, left, right);
    return;
  }

  int64_t smallest = std::numeric_limits<int64_t>::max();
  int64_t biggest = std::numeric_limits<int64_t>::min();
  int64_t bound = partition(arr, left, right + 1, pivot, smallest, biggest);

  if (smallest == biggest)
    return;

#pragma omp task final(bound - left < 10000)
  qs_core(arr, left, bound - 1, average(smallest, pivot));
  qs_core(arr, bound, right, average(pivot, biggest));
}

// wrapper for the quicksort function
void min_max_quicksort(int64_t* arr, int64_t n,
                       int num_threads = omp_get_max_threads()) {
#pragma omp parallel num_threads(num_threads)
#pragma omp single nowait
  qs_core(arr, 0, n - 1, n > 0 ? arr[average(0, n - 1)] : 0);
}

// class for generating random numbers
class Xoroshiro128Plus {
  uint64_t state[2]{};

  static inline uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
  }

 public:
  explicit Xoroshiro128Plus(uint64_t seed = 0) {
    state[0] =
        (12345678901234567 + seed) |
        0b1001000010000001000100101000000110010010100000011001001010000001ULL;
    state[1] =
        (98765432109876543 + seed) |
        0b0100000011001100100000011001001010000000100100101000000110010010ULL;
    for (int i = 0; i < 10; i++) {
      operator()();
    }
  }

  uint64_t operator()() {
    const uint64_t s0 = state[0];
    uint64_t s1 = state[1];
    const uint64_t result = s0 + s1;

    s1 ^= s0;
    state[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16);
    state[1] = rotl(s1, 37);
    return result;
  }
};

bool verify_qs_correctness(int64_t size) {
  Xoroshiro128Plus generator(size);
  std::vector<int64_t> data(size);
  for (int64_t i = 0; i < size; ++i) {
    data[i] = generator();
  }
  std::vector<int64_t> data_copy = data;  // Duplicate for std::sort
  min_max_quicksort(&data[0], size);
  std::sort(data_copy.begin(), data_copy.end());
  return data == data_copy;  // check if arrays are equal
}

void benchmark_threads(const int max_threads, const int64_t repeat,
                       const int64_t size) {
  std::vector<double> speedup_custom(max_threads);
  std::vector<double> speedup_gnu(max_threads);
  std::vector<double> threads(max_threads);

  for (int num_threads = 1; num_threads <= max_threads; num_threads++) {
    threads[num_threads - 1] = num_threads;

    // threads used by min_max_quicksort and __gnu_parallel::sort
    omp_set_num_threads(num_threads);
    std::cout << "Number of threads: " << num_threads << "\n";

    for (int i = 0; i < repeat; i++) {
      std::vector<int64_t> data(size);

      const uint64_t seed = i + num_threads;
      Xoroshiro128Plus generator(seed);

      for (int64_t j = 0; j < size; ++j) {
        data[j] = static_cast<int64_t>(generator());
      }

      // duplicate data
      std::vector<int64_t> data_copy = data;
      std::vector<int64_t> data_copy_parallel = data;

      // measure std::sort
      double start = omp_get_wtime();
      std::sort(data.begin(), data.end());
      const double time_std_sort = omp_get_wtime() - start;

      // measure min_max_quicksort
      start = omp_get_wtime();
      min_max_quicksort(&data_copy[0], size);
      const double time_min_max_quicksort = omp_get_wtime() - start;
      speedup_custom[num_threads - 1] += time_std_sort / time_min_max_quicksort;

      // measure __gnu_parallel::sort
      start = omp_get_wtime();
      __gnu_parallel::sort(data_copy_parallel.begin(), data_copy_parallel.end());
      const double time_gnu_parallel = omp_get_wtime() - start;
      speedup_gnu[num_threads - 1] += time_std_sort / time_gnu_parallel;
    }
    speedup_custom[num_threads - 1] /= static_cast<double>(repeat);
    speedup_gnu[num_threads - 1] /= static_cast<double>(repeat);
  }

  plt::plot(threads, speedup_custom,
  {{"label", "min_max_quicksort"},
    {"color", "darkturquoise"},
    {"marker", "o"},
    {"linewidth", "2"}});
  plt::plot(threads, speedup_gnu,
    {{"label", "gnu_parallel"},
      {"color", "crimson"},
      {"marker", "x"},
      {"linewidth", "2"}});
  plt::xlabel("Number of threads", {{"fontsize", "12"}});
  plt::ylabel("Relative speedup against std::sort", {{"fontsize", "12"}});
  plt::title("Time seedup with varying number of threads", {{"fontsize", "16"}, {"fontweight", "bold"}});
  std::vector<int> xticks = {1, 5, 10, 15, 20};
  plt::xticks(xticks);
  std::vector<int> yticks = {0, 2, 4, 6, 8, 10};
  plt::yticks(yticks);
  plt::legend({{"loc", "upper left"}});
  plt::grid(true);
  PyRun_SimpleString("import matplotlib.pyplot as plt");
  PyRun_SimpleString("plt.grid(which='major', color='gray', linestyle='-', linewidth=0.75)");
  PyRun_SimpleString("plt.grid(which='minor', color='lightgray', linestyle=':', linewidth=0.5)");
  PyRun_SimpleString("plt.minorticks_on()");
  PyRun_SimpleString("plt.axhline(1, lw=1, color='black', linestyle='--')");
  plt::save("seedup_threads.pdf");
  plt::show();
}

void benchmark_size(const std::vector<double>& sizes, const int64_t repeat,
                    const int threads) {
  std::vector<double> speedup_custom(sizes.size());
  std::vector<double> speedup_gnu(sizes.size());
    omp_set_num_threads(threads);
  std::vector<int64_t> data;

  for (int64_t i = 0; i < sizes.size(); ++i) {

    // threads used by min_max_quicksort and __gnu_parallel::sort
    std::cout << "Size: " << sizes[i] << "\n";

    for (int64_t j = 0; j < repeat; j++) {
      const auto size = static_cast<int64_t>(sizes[i]);
      data.resize(size);

      const uint64_t seed = i + j;
      Xoroshiro128Plus generator(seed);

      for (int64_t k = 0; k < size; ++k) {
        data[k] = static_cast<int64_t>(generator());
      }

      // duplicate data
      std::vector<int64_t> data_copy = data;
      std::vector<int64_t> data_copy_parallel = data;

      // measure std::sort
      double start = omp_get_wtime();
      std::sort(data.begin(), data.end());
      const double time_std_sort = omp_get_wtime() - start;

      // measure min_max_quicksort
      start = omp_get_wtime();
      min_max_quicksort(&data_copy[0], size);
      const double time_min_max_quicksort = omp_get_wtime() - start;
      speedup_custom[i] += time_std_sort / time_min_max_quicksort;

      // measure __gnu_parallel::sort
      start = omp_get_wtime();
      __gnu_parallel::sort(data_copy_parallel.begin(), data_copy_parallel.end());
      const double time_gnu_parallel = omp_get_wtime() - start;
      speedup_gnu[i] += time_std_sort / time_gnu_parallel;
    }
    speedup_custom[i] /= static_cast<double>(repeat);
    speedup_gnu[i] /= static_cast<double>(repeat);
  }

  plt::plot(sizes, speedup_custom,
    {{"label", "min_max_quicksort"},
      {"color", "darkturquoise"},
      {"marker", "o"},
      {"linewidth", "2"}});
  plt::plot(sizes, speedup_gnu,
    {{"label", "gnu_parallel"},
      {"color", "crimson"},
      {"marker", "x"},
      {"linewidth", "2"}});
  plt::xlabel("Array size", {{"fontsize", "12"}});
  plt::ylabel("Relative speedup against std::sort", {{"fontsize", "12"}});
  plt::title("Time seedup with varying array sizes", {{"fontsize", "16"}, {"fontweight", "bold"}});
  // std::vector<int> xticks = {1, 5, 10, 15, 20};
  plt::xticks(sizes);
  PyRun_SimpleString("import matplotlib.pyplot as plt");
  PyRun_SimpleString("plt.xscale('log')");
  std::vector<int> yticks = {0, 2, 4, 6, 8, 10};
  plt::yticks(yticks);
  plt::legend({{"loc", "upper left"}});
  plt::grid(true);
  PyRun_SimpleString("plt.grid(which='major', color='gray', linestyle='-', linewidth=0.75)");
  PyRun_SimpleString("plt.grid(which='minor', color='lightgray', linestyle=':', linewidth=0.5)");
  PyRun_SimpleString("plt.minorticks_on()");
  PyRun_SimpleString("plt.axhline(1, lw=1, color='black', linestyle='--')");
  plt::save("seedup_sizes.pdf");
  plt::show();
}

void run_benchmark() {
  // test correctness of min_max_quicksort
  const std::vector<int64_t> sizes = {0, 1, 23, 133, 1777, 57462, 786453};
  for (const auto& size : sizes) {
    if (!verify_qs_correctness(size)) {
      std::cout << "min_max_quicksort is incorrect for size " << size << "!\n";
      exit(1);
    }
  }

  const int MAX_THREADS = omp_get_max_threads();
  // benchmark_threads(MAX_THREADS, 10, 10000000);
  benchmark_size({100, 1000, 10000, 100000, 1000000, 10000000, 100000000}, 10, MAX_THREADS);
}
