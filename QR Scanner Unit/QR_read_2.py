


from pyzbar import pyzbar
import argparse #pasing command line arguements
import cv2
import gspread

read_no =0;
nickey = "";
no_barcoderead =0;
prevkey = "";
change_no = 1;




#load camera
########################
wCam,hCam = 1280, 720   #width and height of camera capture

cap = cv2.VideoCapture(0)   #camera 0
cap.set(3, wCam)    #apply size for the width
cap.set(4, hCam)    #apply size for the height




#database info
sa = gspread.service_account(filename="id_for_atm.json")    #getting the json file
sh = sa.open("Temperature(HMI)") #opening the database
wks = sh.worksheet("Sheet1")    #opening the worksjheet

#2nd database info
sh2 = sa.open("ATMinfo")
wks2 = sh2.worksheet("Sheet1")








while True:
    success, img = cap.read()

    #flip_img = cv2.flip(img, 1)

    barcodes = pyzbar.decode(img)


    #no_barcoderead = 0;

    for barcode in barcodes:
        # extract bounding box location of barcode
        # draw the box
        (x, y, w, h) = barcode.rect
        cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)  # image, point1, point2, color, thickness

        # convert barcode data to string
        barcodeData = barcode.data.decode("utf-8")
        nickey = str(barcodeData)

        if (prevkey != nickey):
            change_no += 1;

        no_barcoderead = 1;
        barcodeType = barcode.type
        read_no += 1;


        #p = 'A'+str(change_no)

        # update the barcode data to google heet
        wks.update('C'+str(change_no),nickey)   #updating the first database
        wks2.update('C'+str(change_no),nickey)    #updaatae the 2nd database

        # draw barcode data and barcode type on image
        text = "{} ({})".format(barcodeData, barcodeType)
        cv2.putText(img, text, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

        # print barcode type and data to terminal
        print("[INFO] Found {} barcode: {}".format(barcodeType, barcodeData))

        prevkey = nickey;




    cv2.imshow("image", img)
    cv2.waitKey(1)




