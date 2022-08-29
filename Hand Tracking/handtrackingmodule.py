#minimum code
import cv2
import mediapipe as mp
import time
import math
import numpy as np


class handDetector():   #create a new class for hand detection
    def __init__(self, mode = False, maxHands=2,detection_con = 0.5, track_con = 0.5):  #initiate the main parameters needed for hand traking
        self.mode = mode
        self.maxHands = maxHands
        self.detection_con = detection_con
        self.track_con = track_con

        self.mpHands = mp.solutions.hands
        self.hands = self.mpHands.Hands(self.mode, self.maxHands, self.detection_con, self.track_con)
        self.detection_con, self.track_con
        self.mpDraw = mp.solutions.drawing_utils #detecting points in a hand

        self.tipIds = [4,8,12,16,20] #fingertip landmark id s

    def findHands(self, img, draw = True):  #(the image, draw the hand skeleton)

        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB) #convert the BGR color to RGB in openCV
        self.results = self.hands.process(imgRGB)
        #print(results.multi_hand_landmarks)

        if self.results.multi_hand_landmarks: #if hand is detected
            for handLms in self.results.multi_hand_landmarks: #handLms use to differ hands if more than 1 hand available
                if draw:
                    self.mpDraw.draw_landmarks(img, handLms, self.mpHands.HAND_CONNECTIONS) #drawing hand landmark dots , drawing the dot connections
        return img  #return the image from the function


    def findposition(self, img, handNo = 0, draw = True):   #(the camera image, which hand using, draw the hand pointer)

        xList = []  #x value list
        yList = []  #y value list
        bbox = []
        #self.lmlist=[]

        x_min = 0
        x_max = 0
        y_min = 0
        y_max = 0

        self.lmList =[]  #the landmarks will be saved in this list
        if self.results.multi_hand_landmarks:  # if hand is detected
            myHand = self.results.multi_hand_landmarks[handNo]  #it will get the hand

            for id, lm in enumerate(myHand.landmark):
                #print(id, lm)
                h, w, c = img.shape #height, width, channel of the image
                cx , cy = int(lm.x*w), int(lm.y*h) #center x,y values                   #it will put the landmaerks in selected hand
                #print(id, cx, cy) #print the new x,y values with their lanmark id
                xList.append(cx)
                yList.append(cy)
                self.lmList.append([id, cx, cy])

                if draw:
                    cv2.circle(img, (cx,cy), 10, (255,0,255),cv2.FILLED) #highlight the position id 0 (img, (position),scale,color)
        if len(xList)>0:
            x_min, x_max = min(xList), max(xList)
        if len(yList)>0:
            y_min, y_max = min(yList), max(yList)
        bbox = x_min, y_min, x_max, y_max

        if draw:
            cv2.rectangle(img, (x_min - 20, y_min - 20), (x_max + 20, y_max + 20), (0, 255, 0), 2)
        return self.lmList, bbox

    def fingersup(self):
        fingers =[]

        #thumb
        if self.lmList[self.tipIds[0]][1] > self.lmList[self.tipIds[0] - 1][1]:
            fingers.append(1)
        else:
            fingers.append(0)

        #04 fingers
        for id in range(1, 5):
            if self.lmList[self.tipIds[id]][2] < self.lmList[self.tipIds[id] - 2][2]:
                fingers.append(1)
            else:
                fingers.append(0)
        return fingers

    def findDistance(self,p1,p2,img, draw=True,r=15, t=3):
        x1, y1 = self.lmList[p1][1:]
        x2, y2 = self.lmList[p2][1:]
        cx, cy = (x1+x2)//2, (y1+y2)//2

        if draw:
            cv2.line(img, (x1,y1), (x2,y2), (255,0,255), t)
            cv2.circle(img, (x1,y1), r, (255,0,255), cv2.FILLED)
            cv2.circle(img, (x2, y2), r, (255, 0, 255), cv2.FILLED)

            cv2.circle(img, (cx, cy), r, (0, 0, 255), cv2.FILLED)
            length = math.hypot(x2 - x1,y2 - y1)

        return length, img, [x1, y1, x2, y2, cx, cy]











def main(): #main function
    pTime = 0  # previous time
    cTime = 0  # current time

    cap = cv2.VideoCapture(0)  # using camera 01
    detector = handDetector()

    while True:
        success, img = cap.read()
        img = detector.findHands(img)

        lmlist = detector.findposition(img)

        #if len(lmList) != 0:
                #print(lmList[8]) #print the landmark values of landmark 8

        cv2.imshow("image", img)
        cv2.waitKey(1)

        cTime = time.time()  # add the current time to the time
        fps = 1 / (cTime - pTime)
        pTime = cTime  # changing the previous time value

        cv2.putText(img, str(int(fps)), (10, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.9,
                    (255, 0, 255), 2)  # ADDING FPS text to the display (position,font style,scale,fontcolor,thickness)



if __name__ == "__main__":
    main()  #run the main function