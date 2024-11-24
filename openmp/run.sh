gcc -pg -o main main.c 
./main 
gprof main gmon.out > analysis.txt
rm gmon.out
rm main