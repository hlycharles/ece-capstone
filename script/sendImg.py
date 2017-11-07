import sys
import serial
from PIL import Image

def processImg(filepath):
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout = 1)
    print(ser.name)
    
    img = Image.open(filepath)
    width, height = img.size
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
            for i in range(len(pixelStr)):
                ser.write(pixelStr[i])
                ser.flush()
            ser.write('*')
            ser.flush()

    s = ser.read(1)
    print s
    ser.close() 
    

if __name__ == "__main__":
    if (len(sys.argv)) < 2:
        print "Please provide image file path"
    processImg(sys.argv[1])
    
