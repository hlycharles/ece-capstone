import sys
import serial
import time
from PIL import Image

def processImg(filepath):
    ser = serial.Serial('/dev/tty.usbmodem1421', 115200, timeout = 2)
    # print(ser.name)
    
    img = Image.open(filepath)
    width, height = img.size
    sentNum = 0
    dataIn = ""

    for y in range(height):
        for x in range(width):
            pixel = img.getpixel((x, y))
            grayPixel = (pixel[0] + pixel[1] + pixel[2]) / 3
            # write pixel
            pixelStr = chr(grayPixel)
            dataIn += pixelStr
            sentNum += 1

    totalTime = 0
    print "sendNum:", sentNum
    for i in range(1):
        ser.write(dataIn)
        ser.flush()
        start = time.time()
        result = ""
        s = ser.read()
        while (s):
            result += s
            if (s == '&'):
                # print "complete: " + str(i)
                break
            s = ser.read()
        end = time.time()
        totalTime += (end - start)
        # print result
    
    print "Complete"
    print "Time: " + str(totalTime)

    ser.close() 
    

if __name__ == "__main__":
    if (len(sys.argv)) < 2:
        print "Please provide image file path"
    processImg(sys.argv[1])
    
