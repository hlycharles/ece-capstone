import sys
import os
from PIL import Image

def decodeImg(filepath, imgpath):
    pixelFile = open(filepath, "r")
    pixelStrs = pixelFile.readlines()
    pixelFile.close()

    pixels = []
    r = int(pixelStrs[0])
    c = int(pixelStrs[1])
    for i in range(2, len(pixelStrs)):
        pixels.append(int(pixelStrs[i]))

    im = Image.new("I", (c, r))
    pix = im.load()
    for x in range(c):
        for y in range(r):
            pix[x, y] = pixels[y * c + x]
    im.convert("RGB").save(imgpath)


if __name__ == "__main__":
    if (len(sys.argv) < 2):
        print "Please provide an txt file to decode"
    if (len(sys.argv) < 3):
        print "Please provide an output path"
    decodeImg(sys.argv[1], sys.argv[2])
