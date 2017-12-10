# get sample images for training

import numpy as np
import cv2
import serial
import time
import subprocess
from PIL import Image


def detec(array):
    args = ["./detection"]
    for row in array:
        for i in row:
            args.append(str(i))
    popen = subprocess.Popen(args, stdout=subprocess.PIPE)
    popen.wait()
    output = popen.stdout.read()
    result = output.split("*")
    return (int(result[0]),int(result[1]),int(result[2]),int(result[3]))


def save_image(x, y, w, h, frame, count):
    sample = frame[y: y + h, x: x + w]
    sample = cv2.resize(sample, (20,20))
    cv2.imwrite("sample_" + str(count) +".jpg", sample)

sample_count = 0
face_cascade = cv2.CascadeClassifier('/home/ece349/Documents/Capstone/opencv/data/haarcascades/haarcascade_frontalface_default.xml')


for i in xrange(1,30):
	filename = "./Lucia/" + str(i) + ".png"
	print filename
	frame = cv2.imread(filename)
	# frame = cv2.resize(frame, (160,120))
	gray = cv2.imread(filename,0)
	gray = cv2.resize(gray, (160,120))
	faces = face_cascade.detectMultiScale(frame, 1.3, 5)
	(x,y,w,h) = (0,0,0,0)
	for (x1,y1,w1,h1) in faces:
		if (w1>w) and (h1>h):
			(x,y,w,h) = (x1,y1,w1,h1)
	if (x + y + w + h) > 0: 
		save_image(x, y, w, h, frame, sample_count)
		sample_count += 1
		print "sample_count: ", sample_count
