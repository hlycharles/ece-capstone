
CC=g++

all: main

main: util/matrix.cpp data/input.cpp data/avg.cpp data/evec.cpp data/eval.cpp recognition.cpp ../resample/resample.cpp main.cpp
	$(CC) util/matrix.cpp data/input.cpp data/avg.cpp data/evec.cpp data/eval.cpp recognition.cpp ../resample/resample.cpp main.cpp -o build/main -lm
