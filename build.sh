rm *o
rm *exe
gcc -g -c glueThread/glthread.c -I glueThread/ -o glueThread/glthread.o
gcc -g -c ThreadTimer.c -I . -I glueThread/ -o ThreadTimer.o
gcc -g -c main.c -I . -o main.o
gcc -g glueThread/glthread.o ThreadTimer.o main.o -o ThreadTimer.exe