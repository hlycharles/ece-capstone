import sys
import serial
import time
import subprocess
from PIL import Image

def processImg(filepath):
    # ser = serial.Serial('/dev/tty.usbmodem1421', 115200, timeout = 2)
    # print(ser.name)
    
    img = Image.open(filepath)
    width, height = img.size
    sentNum = 0
    dataIn = ""

    args = ["../detection/build/detection"]

    for y in range(height):
        for x in range(width):
            pixel = img.getpixel((x, y))
            grayPixel = (pixel[0] + pixel[1] + pixel[2]) / 3
            args.append(str(grayPixel))
            # write pixel
            pixelStr = chr(grayPixel)
            dataIn += pixelStr
            sentNum += 1

    popen = subprocess.Popen(args, stdout=subprocess.PIPE)
    popen.wait()
    output = popen.stdout.read()
    print output

    return

    totalTime = 0
    # print "sendNum:", sentNum
    start = time.time()
    for i in range(1):
        ser.write(dataIn)
        ser.flush()
        result = ""
        s = ser.read()
        while (s):
            if (s == '&'):
                # print "complete: " + str(i)
                break
            result += s
            s = ser.read()
        print result.split('\n')[0]

    end = time.time()
    totalTime += (end - start)
    
    print "Complete"
    print "Time: " + str(totalTime)

    ser.close() 
    

if __name__ == "__main__":
    if (len(sys.argv)) < 2:
        print "Please provide image file path"
    processImg(sys.argv[1])
    
