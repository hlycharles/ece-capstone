#!/bin/bash

# prepare training images
python ../../script/encodeImg.py ./training ./data/imgs.txt ./data/training_size.txt

# prepare input image
python ../../script/encodeImg.py ./input ./data/input.txt
