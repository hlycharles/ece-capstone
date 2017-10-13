# encode images within a give folder and store grayscale values in row major order in a destination file

# command line argument:
# 1) path to folder that contains images to be encoded
# 2) path to a destination file that will store the grayscale pixel values of every image
# 3) OPTIONAL file that stores the total count of encoded images

# the output file will contain one grayscale pixel per line
# e.g. a folder with 2 30x20 images will have an output file with 2x30x20=1200 lines of entry

import sys
import os
from PIL import Image

def isImg(filename):
    return filename.endswith(".jpg")

def saveImg(imgPath, file):
    img = Image.open(imgPath)
    width, height = img.size
    # loop through image horizontally
    for y in range(height):
        for x in range(width):
            pixel = img.getpixel((x, y))
            grayPixel = (pixel[0] + pixel[1] + pixel[2]) / 3
            file.write(str(grayPixel) + "\n")


def encodeImgs(path, destination, imgCountPath):
    file = open(destination, "w")
    imgFiles = []
    for filename in os.listdir(path):
        if (isImg(filename)):
            imgFiles.append(filename)
    if (not path.endswith("/")):
        path += "/"
    imgFiles.sort()
    if (len(imgCountPath) > 0):
        countFile = open(imgCountPath, "w")
        countFile.write(str(len(imgFiles)) + "\n")
        countFile.close()
    for filename in imgFiles:
        saveImg(path + filename, file)
    file.close()

if __name__ == "__main__":
    if (len(sys.argv) < 2):
        print "Please provide a folder that contains images to encode"
    if (len(sys.argv) < 3):
        print "Please provide a destination file to store encoded images"
    imgCountPath = "" if len(sys.argv) < 4 else sys.argv[3]
    if (len(sys.argv) >= 3):
        imgFolderPath = sys.argv[1]
        destination = sys.argv[2]
        encodeImgs(imgFolderPath, destination, imgCountPath)
