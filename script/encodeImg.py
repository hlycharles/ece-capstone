# encode images within a give folder and store grayscale values in row major order in a destination file

# command line argument:
# 1) path to folder that conatains images to be encoded
# 2) path to a destination file that will store the grayscale pixel values of every image

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


def encodeImgs(path, destination):
    file = open(destination, "w")
    for filename in os.listdir(path):
        if (isImg(filename)):
            if (not path.endswith("/")):
                path += "/"
                saveImg(path + filename, file)
    file.close()

if __name__ == "__main__":
    if (len(sys.argv) < 2):
        print "Please provide a folder that contains images to encode"
    if (len(sys.argv) < 3):
        print "Please provide a destination file to store encoded images"
    else:
        imgFolderPath = sys.argv[1]
        destination = sys.argv[2]
        encodeImgs(imgFolderPath, destination)
