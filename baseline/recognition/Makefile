
CC=gcc

all: main train

main: util/matrix.c main.c
	$(CC) -I/usr/local/Cellar/gsl/2.4/include -L/usr/local/Cellar/gsl/2.4/lib util/matrix.c main.c -o build/main -lgsl -lgslcblas -lm

train: util/matrix.c train.c
	$(CC) -I/usr/local/Cellar/gsl/2.4/include -L/usr/loca/Cellar/gsl/2.4/lib util/matrix.c train.c -o build/train -lgsl -lgslcblas -lm
