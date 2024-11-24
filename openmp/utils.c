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
