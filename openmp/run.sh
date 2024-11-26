gcc -fopenmp -pg -o main main.c 
OMP_NUM_THREADS=120 ./main $1
# gprof main gmon.out > analysis.txt
# rm gmon.out
rm main