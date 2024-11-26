#include "utils/header.h"
#include "utils/utils.c"

#include "progs/openmp.c"
#include "progs/base_fix.c"


double bench_t_start, bench_t_end;

void bench_timer_start() { bench_t_start = rtclock(); }

void bench_timer_stop() { bench_t_end = rtclock(); }

void bench_timer_print() {
  printf("Time in seconds: %0.6lf\n", bench_t_end - bench_t_start);
}



static void kernel_heat_3d(int tsteps, int n, float A[n][n][n],
                           float B[n][n][n]) {
  int t, i, j, k;

  for (t = 1; t <= TSTEPS; t++) {
    for (i = 1; i < n - 1; i++) {
      for (j = 1; j < n - 1; j++) {
        for (k = 1; k < n - 1; k++) {
          B[i][j][k] =
              0.125f * (A[i + 1][j][k] - 2.0f * A[i][j][k] + A[i - 1][j][k]) +
              0.125f * (A[i][j + 1][k] - 2.0f * A[i][j][k] + A[i][j - 1][k]) +
              0.125f * (A[i][j][k + 1] - 2.0f * A[i][j][k] + A[i][j][k - 1]) +
              A[i][j][k];
        }
      }
    }

    for (i = 1; i < n - 1; i++) {
      for (j = 1; j < n - 1; j++) {
        for (k = 1; k < n - 1; k++) {
          A[i][j][k] =
              0.125f * (B[i + 1][j][k] - 2.0f * B[i][j][k] + B[i - 1][j][k]) +
              0.125f * (B[i][j + 1][k] - 2.0f * B[i][j][k] + B[i][j - 1][k]) +
              0.125f * (B[i][j][k + 1] - 2.0f * B[i][j][k] + B[i][j][k - 1]) +
              B[i][j][k];
        }
      }
    }
  }
}



int main(int argc, char **argv) {
  if ((argc > 1) && (!strcmp(argv[1], "help"))) {
    print_help();
    return 0;
  }
  int n = N;
  int tsteps = TSTEPS;
  printf("TSTEPS: %d, N: %d\n", tsteps, n);

  float(*A)[n][n][n];
  A = (float(*)[n][n][n])malloc((n) * (n) * (n) * sizeof(float));
  float(*B)[n][n][n];
  B = (float(*)[n][n][n])malloc((n) * (n) * (n) * sizeof(float));
  init_array(n, *A, *B);


  printf("Run model: %s\n", argv[1]);

  bench_timer_start();

  if (argc > 1) {
    if (!strcmp(argv[1], "base")) {
      kernel_heat_3d(tsteps, n, *A, *B);
    } else if (!strcmp(argv[1], "base_fix")) {
      kernel_heat_3d_base_fixed(tsteps, n, *A, *B);
    } else if (!strcmp(argv[1], "openmp_base")) {
      kernel_heat_3d_base_parallel(tsteps, n, *A, *B);
    } else if (!strcmp(argv[1], "openmp_improve")) {
      int blocksize = 10;
      int numthreads = 10;
      kernel_heat_3d_improve_parallel(tsteps, n, *A, *B, blocksize, numthreads);
    }
  } else {
    kernel_heat_3d(tsteps, n, *A, *B);
  }


  bench_timer_stop();
  bench_timer_print();


  // double time_start = omp_get_wtime();

  free((void *)A);
  free((void *)B);

  return 0;
}
