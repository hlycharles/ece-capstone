
CC=gcc

all: main train

main: util/matrix.c main.c
	$(CC) -I/home/luyaoh/gsl/include -L/home/luyaoh/gsl/lib util/matrix.c main.c -o build/main -lgsl -lgslcblas -lm

train: util/matrix.c train.c
	$(CC) -I/home/luyaoh/gsl/include -L/home/luyaoh/gsl/lib util/matrix.c train.c -o build/train -lgsl -lgslcblas -lm
