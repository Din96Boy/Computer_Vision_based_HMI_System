import cv2
import time
import numpy as np
import handtrackingmodule as htm
import math
import pyautogui

#print(pyautogui.size())


pTime = 0;
smoothing_val = 5
plocX, plocY = 0,0
clocX, clocY = 0,0


########################
wCam,hCam = 1280, 720   #width and height of camera capture
wScr, hScr = 1919, 1079 #width and height of screen
frameR = 100

cap = cv2.VideoCapture(1)   #camera 0
cap.set(3, wCam)    #apply size for the width
cap.set(4, hCam)    #apply size for the height

detector = htm.handDetector(maxHands=1)     #hand detecter initiate

while True:
    success, img = cap.read()   #check the success of capture and write the image

    flip_img = cv2.flip(img,1)  #mirror the camera feed
    flip_img = detector.findHands(flip_img)
    lmList, bbox = detector.findposition(flip_img)

    #get tip of index and middle finger
    if len(lmList)!=0:
        x1, y1 = lmList[8][1:]  #for index finger
        x2, y2 = lmList[12][1:]  # for middle finger

        #print(x1, y1, x2, y2)

        #check which fingers are up
        fingers = detector.fingersup()
        #print(fingers)

        cv2.rectangle(flip_img, (frameR, frameR), (wCam - frameR, hCam - frameR), (255, 0, 255), 3)

        #identify index finger : moving mode
        if fingers[1] == 1 and fingers[2] == 0:
            #convert coordinates to the screen size

            x3 = np.interp(x1, (frameR,wCam-frameR),(0,wScr))
            y3 = np.interp(y1,(frameR,hCam-frameR),(0,hScr))

            #smoothen the values
            clocX = plocX + (x3-plocX) / smoothing_val
            clocY = plocY + (y3-plocY) / smoothing_val

            #move the mouse
            pyautogui.moveTo(clocX,clocY) #move move
            cv2.circle(flip_img,(x1,y1),15, (255,255,0), cv2.FILLED)    #enlarge show the tip id when moving
            plocX, plocY = clocX, clocY;

        #index and middle finger up : clicking mode
        if fingers[1] == 1 and fingers[2] == 1:
            length, flip_img, other_info = detector.findDistance(8, 12, flip_img)
            #print(length)

            if length < 40:
                cv2.circle(flip_img,(other_info[4], other_info[5]), 15, (0,255,0), cv2.FILLED)    #for the middle circle for two finger length
                pyautogui.click(button='left', clicks = 1, interval = 0.1)













    #frame rate
    cTime = time.time()
    fps = 1/(cTime - pTime)
    pTime = cTime

    cv2.putText(flip_img, f'FPS: {int(fps)}', (10, 50), cv2.FONT_HERSHEY_COMPLEX, 0.8, (255, 0, 0), 3)  # POSITION,FONT,SCALE,COLOR(B.G.R),THICKNESS

    cv2.imshow("flip_img", flip_img)
    cv2.waitKey(1)



