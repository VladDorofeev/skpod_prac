#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/header.h"
#include "../utils/utils.c"
#include "../progs/base_fix.c"


double bench_t_start, bench_t_end;

void bench_timer_start() { bench_t_start = rtclock(); }

void bench_timer_stop() { bench_t_end = rtclock(); }

void bench_timer_print() {
  printf("Time in seconds: %0.6lf\n", bench_t_end - bench_t_start);
}


void kernel_heat_3d_mpi(int tsteps, int n, float A[n][n][n], float B[n][n][n]) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = n / size;
    int start = rank * local_n;
    int end = (rank == size - 1) ? n : start + local_n;

    // Allocate local arrays
    float (*local_A)[n][n] = malloc(local_n * sizeof(float[n][n]));
    float (*local_B)[n][n] = malloc(local_n * sizeof(float[n][n]));

    // Initialize local arrays
    for (int i = 0; i < local_n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                local_A[i][j][k] = A[start + i][j][k];
                local_B[i][j][k] = B[start + i][j][k];
            }
        }
    }

    for (int t = 1; t <= tsteps; t++) {
        // Update local_B
        for (int i = 1; i < local_n - 1; i++) {
            for (int j = 1; j < n - 1; j++) {
                for (int k = 1; k < n - 1; k++) {
                    double residue;
                    local_B[i][j][k] = local_A[i][j][k] * 0.25;
                    residue = local_A[i + 1][j][k] + local_A[i - 1][j][k] + local_A[i][j + 1][k] +
                              local_A[i][j - 1][k] + local_A[i][j][k + 1] + local_A[i][j][k - 1];
                    residue *= 0.125;
                    local_B[i][j][k] += residue;
                }
            }
        }

        // Exchange boundaries for local_B
        MPI_Request send_request[2], recv_request[2];
        MPI_Status status[2];

        if (rank > 0) {
            MPI_Irecv(&local_B[0][0][0], n * n, MPI_FLOAT, rank - 1, 0, MPI_COMM_WORLD, &recv_request[0]);
            MPI_Isend(&local_B[1][0][0], n * n, MPI_FLOAT, rank - 1, 0, MPI_COMM_WORLD, &send_request[0]);
        }
        if (rank < size - 1) {
            MPI_Irecv(&local_B[local_n - 1][0][0], n * n, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD, &recv_request[1]);
            MPI_Isend(&local_B[local_n - 2][0][0], n * n, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD, &send_request[1]);
        }

        // Wait for completion of non-blocking operations
        if (rank > 0) {
            MPI_Wait(&recv_request[0], &status[0]);
            MPI_Wait(&send_request[0], &status[0]);
        }
        if (rank < size - 1) {
            MPI_Wait(&recv_request[1], &status[1]);
            MPI_Wait(&send_request[1], &status[1]);
        }

        // Update local_A
        for (int i = 1; i < local_n - 1; i++) {
            for (int j = 1; j < n - 1; j++) {
                for (int k = 1; k < n - 1; k++) {
                    double residue;
                    local_A[i][j][k] = local_B[i][j][k] * 0.25;
                    residue = local_B[i + 1][j][k] + local_B[i - 1][j][k] + local_B[i][j + 1][k] +
                              local_B[i][j - 1][k] + local_B[i][j][k + 1] + local_B[i][j][k - 1];
                    residue *= 0.125;
                    local_A[i][j][k] += residue;
                }
            }
        }

        // Exchange boundaries for local_A
        if (rank > 0) {
            MPI_Irecv(&local_A[0][0][0], n * n, MPI_FLOAT, rank - 1, 0, MPI_COMM_WORLD, &recv_request[0]);
            MPI_Isend(&local_A[1][0][0], n * n, MPI_FLOAT, rank - 1, 0, MPI_COMM_WORLD, &send_request[0]);
        }
        if (rank < size - 1) {
            MPI_Irecv(&local_A[local_n - 1][0][0], n * n, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD, &recv_request[1]);
            MPI_Isend(&local_A[local_n - 2][0][0], n * n, MPI_FLOAT, rank + 1, 0, MPI_COMM_WORLD, &send_request[1]);
        }

        // Wait for completion of non-blocking operations
        if (rank > 0) {
            MPI_Wait(&recv_request[0], &status[0]);
            MPI_Wait(&send_request[0], &status[0]);
        }
        if (rank < size - 1) {
            MPI_Wait(&recv_request[1], &status[1]);
            MPI_Wait(&send_request[1], &status[1]);
        }
    }

    // Gather results
    MPI_Gather(local_A, local_n * n * n, MPI_FLOAT, A, local_n * n * n, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Gather(local_B, local_n * n * n, MPI_FLOAT, B, local_n * n * n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Free local arrays
    free(local_A);
    free(local_B);
}


int main(int argc, char *argv[]) {
    // float(*C)[n][n][n];
    // C = (float(*)[n][n][n])malloc((n) * (n) * (n) * sizeof(float));
    // float(*D)[n][n][n];
    // D = (float(*)[n][n][n])malloc((n) * (n) * (n) * sizeof(float));
    // init_array(n, *C, *D);


    MPI_Init(&argc, &argv);

    int n = N;
    int tsteps = TSTEPS;
    printf("TSTEPS: %d, N: %d\n", tsteps, n);

    float(*A)[n][n][n];
    A = (float(*)[n][n][n])malloc((n) * (n) * (n) * sizeof(float));
    float(*B)[n][n][n];
    B = (float(*)[n][n][n])malloc((n) * (n) * (n) * sizeof(float));
    init_array(n, *A, *B);

    bench_timer_start();
    kernel_heat_3d_mpi(tsteps, n, *A, *B);
    bench_timer_stop();
    bench_timer_print();

    // if (check_array_equal(n, A,B,C,D)) {
    //     print("GOOD REALIZATION\n");
    // } else {
    //     print("BAD!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    // }

    // Free global arrays
    free(A);
    free(B);

    MPI_Finalize();
    // free(C);
    // free(D);
    return 0;
}
