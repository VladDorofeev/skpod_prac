#define MIN(x, y) (x < y ? x : y)

static void kernel_heat_3d_base_parallel(int tsteps, int n, float A[n][n][n],
                                         float B[n][n][n]) {
  int t, i, j, k;

  for (t = 1; t <= tsteps; t++) {

#pragma omp parallel for private(i, j, k)

    for (i = 1; i < n - 1; i++) {
      for (j = 1; j < n - 1; j++) {
        for (k = 1; k < n - 1; k++) {
          double residue;
          B[i][j][k] = A[i][j][k] * 0.25;
          residue = A[i + 1][j][k] + A[i - 1][j][k] + A[i][j + 1][k] +
                    A[i][j - 1][k] + A[i][j][k + 1] + A[i][j][k - 1];
          residue *= 0.125;
          B[i][j][k] += residue;
        }
      }
    }

#pragma omp parallel for private(i, j, k)

    for (i = 1; i < n - 1; i++) {
      for (j = 1; j < n - 1; j++) {
        for (k = 1; k < n - 1; k++) {
          double residue;
          A[i][j][k] = B[i][j][k] * 0.25;
          residue = B[i + 1][j][k] + B[i - 1][j][k] + B[i][j + 1][k] +
                    B[i][j - 1][k] + B[i][j][k + 1] + B[i][j][k - 1];
          residue *= 0.125;
          A[i][j][k] += residue;
        }
      }
    }
  }
}

// Параллельная - для небольших датасетов
static void kernel_heat_3d_parallel_mini(int tsteps, int n, float A[n][n][n],
                                         float B[n][n][n]) {
  int t, i, j, k;

  for (t = 1; t <= tsteps; t++) {

#pragma omp parallel for private(i, j, k)
    for (i = 1; i < n - 1; i++) {
      for (j = 1; j < n - 1; j++) {
        for (k = 1; k + 3 < n - 1; k += 4) {
          double residue;
          B[i][j][k] = A[i][j][k] * 0.25;
          residue = A[i + 1][j][k] + A[i - 1][j][k] + A[i][j + 1][k] +
                    A[i][j - 1][k] + A[i][j][k + 1] + A[i][j][k - 1];
          residue *= 0.125;
          B[i][j][k] += residue;

          B[i][j][k + 1] = A[i][j][k + 1] * 0.25;
          residue = A[i + 1][j][k + 1] + A[i - 1][j][k + 1] +
                    A[i][j + 1][k + 1] + A[i][j - 1][k + 1] + A[i][j][k + 2] +
                    A[i][j][k];
          residue *= 0.125;
          B[i][j][k + 1] += residue;

          B[i][j][k + 2] = A[i][j][k + 2] * 0.25;
          residue = A[i + 1][j][k + 2] + A[i - 1][j][k + 2] +
                    A[i][j + 1][k + 2] + A[i][j - 1][k + 2] + A[i][j][k + 3] +
                    A[i][j][k + 1];
          residue *= 0.125;
          B[i][j][k + 2] += residue;

          B[i][j][k + 3] = A[i][j][k + 3] * 0.25;
          residue = A[i + 1][j][k + 3] + A[i - 1][j][k + 3] +
                    A[i][j + 1][k + 3] + A[i][j - 1][k + 3] + A[i][j][k + 4] +
                    A[i][j][k + 2];
          residue *= 0.125;
          B[i][j][k + 3] += residue;
        }
      }
    }

#pragma omp parallel for private(i, j, k)
    for (i = 1; i < n - 1; i++) {
      for (j = 1; j < n - 1; j++) {
        for (k = 1; k + 3 < n - 1; k += 4) {
          double residue;
          A[i][j][k] = B[i][j][k] * 0.25;
          residue = B[i + 1][j][k] + B[i - 1][j][k] + B[i][j + 1][k] +
                    B[i][j - 1][k] + B[i][j][k + 1] + B[i][j][k - 1];
          residue *= 0.125;
          A[i][j][k] += residue;

          A[i][j][k + 1] = B[i][j][k + 1] * 0.25;
          residue = B[i + 1][j][k + 1] + B[i - 1][j][k + 1] +
                    B[i][j + 1][k + 1] + B[i][j - 1][k + 1] + B[i][j][k + 2] +
                    B[i][j][k];
          residue *= 0.125;
          A[i][j][k + 1] += residue;

          A[i][j][k + 2] = B[i][j][k + 2] * 0.25;
          residue = B[i + 1][j][k + 2] + B[i - 1][j][k + 2] +
                    B[i][j + 1][k + 2] + B[i][j - 1][k + 2] + B[i][j][k + 3] +
                    B[i][j][k + 1];
          residue *= 0.125;
          A[i][j][k + 2] += residue;

          A[i][j][k + 3] = B[i][j][k + 3] * 0.25;
          residue = B[i + 1][j][k + 3] + B[i - 1][j][k + 3] +
                    B[i][j + 1][k + 3] + B[i][j - 1][k + 3] + B[i][j][k + 4] +
                    B[i][j][k + 2];
          residue *= 0.125;
          A[i][j][k + 3] += residue;
        }
      }
    }
  }
}

// Параллельная для больших датасетов при сравнительно небольшом числе нитей
static void kernel_heat_3d_parallel_normal(int tsteps, int n, float A[n][n][n],
                                           float B[n][n][n], int block_size) {
  int t, i, j, k, kk, jj;

  for (t = 1; t <= tsteps; t++) {

#pragma omp parallel for private(i, j, k, kk, jj)
    for (jj = 1; jj < n - 1; jj += block_size) {
      for (i = 1; i < n - 1; i++) {
        for (kk = 1; kk < n - 1; kk += block_size) {
          for (j = jj; j < MIN(n - 1, jj + block_size); j++) {
            for (k = kk; k < MIN(n - 1, kk + block_size); k++) {
              double residue;
              B[i][j][k] = A[i][j][k] * 0.25;
              residue = A[i + 1][j][k] + A[i - 1][j][k] + A[i][j + 1][k] +
                        A[i][j - 1][k] + A[i][j][k + 1] + A[i][j][k - 1];
              residue *= 0.125;
              B[i][j][k] += residue;
            }
          }
        }
      }
    }

#pragma omp parallel for private(i, j, k, kk, jj)
    for (jj = 1; jj < n - 1; jj += block_size) {
      for (i = 1; i < n - 1; i++) {
        for (kk = 1; kk < n - 1; kk += block_size) {
          for (j = jj; j < MIN(n - 1, jj + block_size); j++) {
            for (k = kk; k < MIN(n - 1, kk + block_size); k++) {
              double residue;
              A[i][j][k] = B[i][j][k] * 0.25;
              residue = B[i + 1][j][k] + B[i - 1][j][k] + B[i][j + 1][k] +
                        B[i][j - 1][k] + B[i][j][k + 1] + B[i][j][k - 1];
              residue *= 0.125;
              A[i][j][k] += residue;
            }
          }
        }
      }
    }
  }
}

// Параллельная для больших датасетов при сравнительно большом числе нитей
static void kernel_heat_3d_parallel_big(int tsteps, int n, float A[n][n][n],
                                        float B[n][n][n], int block_size) {
  int t, i, j, k, kk, jj;

  for (t = 1; t <= tsteps; t++) {

#pragma omp parallel private(i, j, k, kk, jj)
    for (i = 1; i < n - 1; i++) {
      for (jj = 1; jj < n - 1; jj += block_size) {
        for (kk = 1; kk < n - 1; kk += block_size) {
          for (j = jj; j < MIN(n - 1, jj + block_size); j++) {
            for (k = kk; k < MIN(n - 1, kk + block_size); k++) {
              double residue;
              B[i][j][k] = A[i][j][k] * 0.25;
              residue = A[i + 1][j][k] + A[i - 1][j][k] + A[i][j + 1][k] +
                        A[i][j - 1][k] + A[i][j][k + 1] + A[i][j][k - 1];
              residue *= 0.125;
              B[i][j][k] += residue;
            }
          }
        }
      }
    }

#pragma omp parallel private(i, j, k, kk, jj)
    for (i = 1; i < n - 1; i++) {
      for (jj = 1; jj < n - 1; jj += block_size) {
        for (kk = 1; kk < n - 1; kk += block_size) {
          for (j = jj; j < MIN(n - 1, jj + block_size); j++) {
            for (k = kk; k < MIN(n - 1, kk + block_size); k++) {
              double residue;
              A[i][j][k] = B[i][j][k] * 0.25;
              residue = B[i + 1][j][k] + B[i - 1][j][k] + B[i][j + 1][k] +
                        B[i][j - 1][k] + B[i][j][k + 1] + B[i][j][k - 1];
              residue *= 0.125;
              A[i][j][k] += residue;
            }
          }
        }
      }
    }
  }
}

static void kernel_heat_3d_improve_parallel(int tsteps, int n, float A[n][n][n],
                                            float B[n][n][n], int block_size,
                                            int count_threads) {
  if (n < 40) {
    // небольшой датасет
    kernel_heat_3d_parallel_mini(tsteps, n, A, B);
  } else if ((n >= 40) && count_threads <= n / block_size) {
    // большой датасет и число нитей позволяют распараллелить по блокам
    kernel_heat_3d_parallel_normal(tsteps, n, A, B, block_size);
  } else {
    // большой датасет, а число нитей позволяет распараллелить внешний цикл
    kernel_heat_3d_parallel_big(tsteps, n, A, B, block_size);
  }
}

static void kernel_heat_3d_task_parallel(int tsteps, int n, float A[n][n][n],
                                         float B[n][n][n]) {
  int t, i, j, k;

  for (t = 1; t <= tsteps; t++) {

#pragma omp parallel private(i, j, k) //shared(A, B)
    {
#pragma omp single
      {
        for (i = 1; i < n - 1; i++) {
#pragma omp task private(j, k) firstprivate(i) //shared(A, B)
          {
            for (j = 1; j < n - 1; j++) {
              for (k = 1; k < n - 1; k++) {
                double residue;
                B[i][j][k] = A[i][j][k] * 0.25;
                residue = A[i + 1][j][k] + A[i - 1][j][k] + A[i][j + 1][k] +
                          A[i][j - 1][k] + A[i][j][k + 1] + A[i][j][k - 1];
                residue *= 0.125;
                B[i][j][k] += residue;
              }
            }
          }
        }
      }
    }

#pragma omp parallel private(i, j, k) //shared(A, B)
    {
#pragma omp single
      {
        for (i = 1; i < n - 1; i++) {
#pragma omp task private(j, k) firstprivate(i) //shared(A, B)
          {
            for (j = 1; j < n - 1; j++) {
              for (k = 1; k < n - 1; k++) {
                double residue;
                A[i][j][k] = B[i][j][k] * 0.25;
                residue = B[i + 1][j][k] + B[i - 1][j][k] + B[i][j + 1][k] +
                          B[i][j - 1][k] + B[i][j][k + 1] + B[i][j][k - 1];
                residue *= 0.125;
                A[i][j][k] += residue;
              }
            }
          }
        }
      }
    }
  }
}
