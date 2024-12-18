/* Include benchmark-specific header. */
#include "header.h"

static int check_array_equal(int n, float A[n][n][n], float B[n][n][n],
                             float C[n][n][n], float D[n][n][n]) {
  int i, j, k;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      for (k = 0; k < n; k++) {
        if (A[i][j][k] != C[i][j][k]) {
          return 1;
        }
        if (B[i][j][k] != D[i][j][k]) {
          return 1;
        }
      }
    }
  }
  return 0;
}

static void print_array(int n, float A[n][n][n]) {
  int i, j, k;

  fprintf(stdout, "==BEGIN DUMP_ARRAYS==\n");
  fprintf(stdout, "begin dump: %s", "A");
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      for (k = 0; k < n; k++) {
        if ((i * n * n + j * n + k) % 20 == 0)
          fprintf(stdout, "\n");
        fprintf(stdout, "%0.2f ", A[i][j][k]);
      }
  fprintf(stdout, "\nend   dump: %s\n", "A");
  fprintf(stdout, "==END   DUMP_ARRAYS==\n");
}

static double rtclock() {
  struct timeval Tp;
  int stat;
  stat = gettimeofday(&Tp, NULL);
  if (stat != 0)
    printf("Error return from gettimeofday: %d", stat);
  return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}


static void init_array(int n, float A[n][n][n], float B[n][n][n]) {
  int i, j, k;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      for (k = 0; k < n; k++)
        A[i][j][k] = B[i][j][k] = (float)(i + j + (n - k)) * 10 / (n);
}

static void print_help(void) {
  printf("============ Program for parallel ============\n\n");
  printf("|---------------------------------------|\n");
  printf("| DATASET_NAME         |  TSTEPS |  N   |\n");
  printf("|---------------------------------------|\n");
  printf("| MINI_DATASET         |   20    |  10  |\n");
  printf("| SMALL_DATASET        |   40    |  20  |\n");
  printf("| MEDIUM_DATASET       |   100   |  40  |\n");
  printf("| LARGE_DATASET        |   500   |  120 |\n");
  printf("| EXTRALARGE_DATASET   |   1000  |  200 |\n");
  printf("|=======================================|\n\n");
  printf("Programs:\n");
  printf("base programm\n");
  printf("base_fix programm\n");
  printf("openmp_base programm\n");
  printf("openmp_improve programm\n");
  printf("openmp_task\n");
}