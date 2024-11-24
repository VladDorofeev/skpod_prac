gcc -fopenmp -pg -o main main.c 
OMP_NUM_THREADS=120 ./main 
gprof main gmon.out > analysis.txt
rm gmon.out
rm main