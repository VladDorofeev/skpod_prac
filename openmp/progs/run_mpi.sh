rm *.out
rm *.err
mpicc -O0 -o mpi_run -D $1 mpi.c 
mpisubmit.pl -p $2 --stdout mpi_run.out --stderr /dev/null mpi_run