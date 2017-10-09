
CC=gcc

all: main train test-matrix

main: util/io.c util/matrix.c main.c
	$(CC) -I/usr/local/Cellar/gsl/2.4/include -L/usr/local/Cellar/gsl/2.4/lib util/io.c util/matrix.c main.c -o build/main -lgsl -lgslcblas -lm

train: util/io.c util/matrix.c train.c
	$(CC) -I/usr/local/Cellar/gsl/2.4/include -L/usr/local/Cellar/gsl/2.4/lib util/io.c util/matrix.c train.c -o build/train -lgsl -lgslcblas -lm

test-matrix: util/io.c util/matrix.c util/tests/main.c
	$(CC) -I/usr/local/Cellar/gsl/2.4/include -L/usr/local/Cellar/gsl/2.4/lib util/io.c util/matrix.c util/tests/main.c -o build/test-matrix -lgsl -lgslcblas -lm
