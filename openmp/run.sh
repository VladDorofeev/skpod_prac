gcc -fopenmp -p -Ofast -o main -D $1 main.c 
OMP_NUM_THREADS=$3 ./main $2 > temp.txt
rm main
