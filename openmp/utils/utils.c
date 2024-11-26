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

  fprintf(stderr, "==BEGIN DUMP_ARRAYS==\n");
  fprintf(stderr, "begin dump: %s", "A");
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      for (k = 0; k < n; k++) {
        if ((i * n * n + j * n + k) % 20 == 0)
          fprintf(stderr, "\n");
        fprintf(stderr, "%0.2f ", A[i][j][k]);
      }
  fprintf(stderr, "\nend   dump: %s\n", "A");
  fprintf(stderr, "==END   DUMP_ARRAYS==\n");
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
