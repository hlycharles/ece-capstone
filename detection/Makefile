
CC=g++

all: main

main: ../recognition/util/matrix.cpp ../recognition/data/input.cpp ../recognition/data/avg.cpp ../recognition/data/evec.cpp ../recognition/data/eval.cpp ../recognition/recognition.cpp ../resample/resample.cpp ../recognition/recognitionModule.cpp ./sqrt.cpp ./120p.cpp ./haar_mapping.cpp ./haar_ewc.cpp ./haar_rcc.cpp ./image.cpp ./haar.cpp ./main.cpp
	$(CC) ../recognition/util/matrix.cpp ../recognition/data/input.cpp ../recognition/data/avg.cpp ../recognition/data/evec.cpp ../recognition/data/eval.cpp ../recognition/recognition.cpp ../resample/resample.cpp ../recognition/recognitionModule.cpp ./sqrt.cpp ./120p.cpp ./haar_mapping.cpp ./haar_ewc.cpp ./haar_rcc.cpp ./image.cpp ./haar.cpp ./main.cpp -o build/detection -lm
