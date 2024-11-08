#include <algorithm>
#include <cstring>
#include <iostream>
#include <omp.h>
#include <random>
#include "coding_warmup.h"
using namespace std;

void insertion_sort(int *arr, int n) {
  for (int i = 1; i < n; i++) {
    int key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

inline void merge(const int *__restrict__ a, const int *__restrict__ b,
                  int *__restrict__ c, const int a_size, const int b_size,
                  const int c_size) {
  int idx_a = 0;
  int idx_b = 0;
  for (int i = 0; i < c_size; ++i) {
    if (idx_a == a_size) {
      c[i] = b[idx_b++];
    } else if (idx_b == b_size) {
      c[i] = a[idx_a++];
    } else {
      c[i] = (a[idx_a] < b[idx_b]) ? a[idx_a++] : b[idx_b++];
    }
  }
}

void merge_sort(int *arr, int n) {
  if (n < 64) {
    insertion_sort(arr, n);
    return;
  }
  const int size_a = n / 2;
  const int size_b = n - size_a;
#pragma omp task final(size_a < 10000)
  merge_sort(arr, size_a); // recursive call
#pragma omp task final(size_b < 10000)
  merge_sort(arr + size_a, size_b); // recursive call
#pragma omp taskwait
  if (n < 8192) {
    int *c = (int*) alloca(n * sizeof(int));
    merge(arr, arr + size_a, c, size_a, size_b, n);
    memcpy(arr, c, sizeof(int) * n);
    return;
  }
  int *c = new int[n];
  merge(arr, arr + size_a, c, size_a, size_b, n);
  memcpy(arr, c, sizeof(int) * n);
  delete[](c);
}

void run_merge_sort() {
  cout << "Run merge sort:\n";
  const int n = 10000000;
  vector<int> v = get_random_int_vector(n);
  vector<int> v_copy = v;

  double start = omp_get_wtime();
#pragma omp parallel
#pragma omp single nowait
  merge_sort(v.data(), n);
  cout << "parallel: " << omp_get_wtime() - start << " seconds" << endl;

  start = omp_get_wtime();
  sort(begin(v_copy), end(v_copy));
  cout << "std::sort: " << omp_get_wtime() - start << " seconds" << endl;

  if (v != v_copy) {
    cout << "sort implementation is buggy\n";
  }
}
