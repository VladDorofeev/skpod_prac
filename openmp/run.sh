gcc -fopenmp -pg -o main -D $1 main.c 
OMP_NUM_THREADS=120 ./main $2 > temp.txt
rm gmon.out
rm main