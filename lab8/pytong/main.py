import cv2

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if ret == True:
        frame = cv2.flip(frame, 1)
        cv2.imshow('frame', frame)
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY) #Grayscale
        edges = cv2.Canny(gray, 50, 150, apertureSize = 3) #Canny Edge Detection
        cv2.imshow('edges', edges)
        t1 = cv2.getTickCount()
        t2 = cv2.getTickCount()
        time = (t2 - t1)/ cv2.getTickFrequency()
        print(time)
        if cv2.waitKey(1) & 0xFF == 27:
            break

cap.release()
cv2.destroyAllWindows()

