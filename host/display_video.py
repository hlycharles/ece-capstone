import numpy as np
import cv2
import serial
import time
from PIL import Image

def detec(img):
    for row in img:
          for i in row:
              ser.write(chr(i))
        ser.flush()
        buf = ""
        while (True):
            s = ser.read(1)
            # end of result
            if s == '&':
                break
            else:
                buf += s
    array = buf.split("\n")
    faceId = int(array[0])
    fx = int(array[1])
    fy = int(array[2])
    fw = int(array[3])
    fh = int(array[4])
    return (faceId, fx, fy, fw, fh)


# Save training samples
def save_image(x, y, w, h, frame, count):
    sample = frame[y: y + h, x: x + w]
    sample = cv2.resize(sample, (20,20))
    cv2.imwrite("sample_" + str(count) +".jpg", sample)

cap = cv2.VideoCapture(0)

ser = serial.Serial('/dev/ttyACM0', 115200, timeout = 3)
print(ser.name)
cap.set(cv2.CAP_PROP_FPS, 10)
print(cap.get(5))
face_cascade = cv2.CascadeClassifier('/home/ece349/Documents/Capstone/opencv/data/haarcascades/haarcascade_frontalface_default.xml')
sample_count = 0
our_counter = 0
cv_counter = 0

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(cv2.resize(gray, (160,120)), 1.3, 5)
    (faceId, fx, fy, fw, fh) = detec(cv2.resize(gray, (160,120)))
    # scale to fit original frame size
    fx = fx * 4
    fy = fy * 4
    fw = fw * 4
    fh = fh * 4
    if (fx + fy + fw + fh) > 0:
        our_counter += 1
        cv2.rectangle(frame,(fx,fy),(fx+fw,fy+fh),(0,255,0),2)
        text = ""
        if (faceId >= 0 and faceId <= 24):
            text = "Jason"
        elif (faceId >= 25 and faceId <= 49):
            text = "Luyao"
        else:
            text = "Haocheng"
        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(frame, text, (110, 100), font, 1, (0, 255, 0), 2, cv2.LINE_AA)

    (x,y,w,h) = (0,0,0,0)
    for (x1,y1,w1,h1) in faces:
        if (w1>w) and (h1>h):
            (x,y,w,h) = (x1,y1,w1,h1)
    if (x + y + w + h) > 0: 
        cv_counter += 1
        cv2.rectangle(frame,(4*x,4*y),(4*x+4*w,4*y+4*h),(0,0,255),2)

    font = cv2.FONT_HERSHEY_SIMPLEX
    cv2.putText(frame, str(our_counter), (410, 100), font, 0.5, (0, 255, 0), 2, cv2.LINE_AA)
    cv2.putText(frame, str(cv_counter), (510, 100), font, 0.5, (0, 0, 255), 2, cv2.LINE_AA)
    cv2.imshow("image", frame)
    cv2.waitKey(1)

# When everything done, release the capture
ser.close()
cap.release()
cv2.waitKey(0)
cv2.destroyAllWindows()
