
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