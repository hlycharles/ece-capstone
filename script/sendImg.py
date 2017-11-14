import sys
import serial
from PIL import Image

def processImg(filepath):
    ser = serial.Serial('/dev/tty.usbmodem1421', 115200, timeout = 2)
    print(ser.name)
    
    img = Image.open(filepath)
    width, height = img.size
    sentNum = 0
    dataIn = ""

    for y in range(height):
        for x in range(width):
            pixel = img.getpixel((x, y))
            grayPixel = (pixel[0] + pixel[1] + pixel[2]) / 3
            # write pixel
            pixelStr = ""
            while (grayPixel >= 10):
                pixelStr = str(grayPixel % 10) + pixelStr
                grayPixel /= 10
            pixelStr = str(grayPixel) + pixelStr
            dataIn += pixelStr
            for i in range(len(pixelStr)):
                ser.write(pixelStr[i])
                ser.flush()
            ser.write('*')
            sentNum += 1
            ser.flush()
            dataIn += "*"

    print "sent:", sentNum
    result = ""
    s = ser.read()
    while (s):
        result += s
        s = ser.read()
    print result
    ser.close() 
    

if __name__ == "__main__":
    if (len(sys.argv)) < 2:
        print "Please provide image file path"
    processImg(sys.argv[1])
    
