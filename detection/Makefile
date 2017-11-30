
CC=g++

all: main

main: ../recognition/util/matrix.cpp ../recognition/data/input.cpp ../recognition/data/avg.cpp ../recognition/data/evec.cpp ../recognition/data/eval.cpp ../recognition/recognition.cpp ../resample/resample.cpp ../recognition/recognitionModule.cpp ./image.cpp ./haar.cpp ./main-r.cpp
	$(CC) ../recognition/util/matrix.cpp ../recognition/data/input.cpp ../recognition/data/avg.cpp ../recognition/data/evec.cpp ../recognition/data/eval.cpp ../recognition/recognition.cpp ../resample/resample.cpp ../recognition/recognitionModule.cpp ./image.cpp ./haar.cpp ./main-r.cpp -o build/detection -lm
