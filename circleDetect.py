import subprocess
import sys
# def _install(package):
#     subprocess.check_call([sys.executable, '-m', 'pip3', 'install', package])

# install_packages = ["_install(opencv)","_install(numpy)"]
# # install_packages = ["_install(pyserial)","_install(cv2)","_install(numpy)"]
# # import_packages = ["import pyserial", "import cv2", "import numpy as np"]
# import_packages = ["import opencv", "import numpy as np"]

# for package in range(3):
#     try:
#         exec(import_packages[package])
#     except:
#         print("pyserial is not installed. Installing now...")
#         exec(install_packages[package])
#         exec(import_packages[package])

# TEST
import cv2
import numpy as np
import os

def nothing():
    pass

def DetectCircle(file_name):
    title = f"image: {file_name}"
    folder_name = 'images'
    print(title)
    #loading file
    last = os.path.join(os.getcwd(), folder_name)
    file_name = os.path.join(last, file_name)
    print(file_name)
    image = cv2.imread(file_name)
    print(type(image))

    #converting to gray scale
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, thresh = cv2.threshold(gray_image,127,255, cv2.THRESH_BINARY_INV)
    cv2.imshow("Inverted",thresh)

    #load bar
    cv2.createTrackbar("track bar","track",0,255,nothing)
    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    #filter max contour
    c = max(contours, key = cv2.contourArea) #get greatest area curve
    area = cv2.contourArea(c)
    area_title = f"area =  {area}"
    radius_title = f"radius = {np.sqrt(area/np.pi):.2f}"
    cv2.drawContours(image, c, -1, (0,0,255), 4)
    x,y,w,h = cv2.boundingRect(c)
    x_mid = int(x+w/2)
    y_mid = int(y+h/2)
    coords = (x_mid, y_mid)
    colour = (0,255,0)
    font = cv2.FONT_HERSHEY_PLAIN
    cv2.putText(image,title, (1,20), font, 1, (0,0,0), 1)
    cv2.putText(image,"circle", coords, font, 1, colour, 1)
    cv2.putText(image, area_title, (coords[0]-200, max(0,coords[1]+15)), font, 1, colour, 1)
    cv2.putText(image, radius_title, (coords[0]-200, max(0,coords[1]+30)), font, 1, colour, 1)
    # going through all contours
    # for i, contour in enumerate(contours):
    #     if i ==0: # first shape, i.e. the whole shape
    #         continue
    #     # epsilon = 0.01 * cv2.arcLength(contour, True)
    #     # approx = cv2.approxPolyDP(contour, epsilon, True)
        
    # #     #trying blob fit
    # #     # params = cv2.SimpleBlobDetector_Params()
    # #     # params.minThreshold = 127
    # #     # params.maxThreshold = 255
    # #     # params.filterByArea = True
    # #     # params.minArea = 1500
    # #     # params.filterByCircularity = True
    # #     # params.minCircularity = 0.1
    # #     # params.filterByConvexity = True
    # #     # params.minConvexity = 0.87
    # #     # params.filterByInertia = True
    # #     # params.minInertiaRatio = 0.01

    # #     # ver = (cv2.__version__).split('.')
    # #     # if int(ver[0]) < 3:
    # #     #     detector = cv2.SimpleBlobDetector(params)
    # #     # else:
    # #     #     detector = cv2.SimpleBlobDetector_create(params)

    # #     # points = detector.detect(thresh)

    # #     # display_points = cv2.drawKeypoints(thresh,points,np.array([]),(255,0,0), cv2.DrawMatchesFlags_DRAW_RICH_KEYPOINTS)

    # #     # cv2.imshow("bruh",display_points)
    # #     (x,y), radius = cv2.minEnclosingCircle(contour)
    # #     circle = cv2.circle(thresh, (int(x),int(y)), int(radius), (0,255,0), 2)
    # #     cv2.imshow("bruh", circle)

    #     area = cv2.contourArea(contour)
    #     print("image: ", file_name,", area = ", area)
    #     print("radius = ", np.sqrt(area/np.pi))
    #     if area>6000:
    #         cv2.drawContours(image, contour, -1, (0,0,255), 4)

    #     # x,y,w,h = cv2.boundingRect(approx)
    #     # x_mid = int(x+w/2)
    #     # y_mid = int(y+h/2)

    #     # coords = (x_mid, y_mid)
    #     # colour = (0,0,0)
    #     # font = cv2.FONT_HERSHEY_PLAIN

    #     # if len(approx>10):
    #     #     cv2.putText(image,"circle", coords, font, 1, colour, 1)
    #     # if len(approx<5):
    #     #     cv2.putText(image,"square", coords, font, 1, colour, 1)

    cv2.imshow("output", image)
    cv2.waitKey(0)

for files in os.listdir("Images"):
    if files.endswith("bmp"):
        DetectCircle(files)
    else:
        print(os.getcwd())
        print("nothing")
    
# Main program part
    
# import serial
# import subprocess

# # Open serial port
# ser = serial.Serial('/dev/ttyACM0', 9600)  # Adjust port and baud rate as needed

# # Listen for commands from Arduino
# done = False
# while True:
#     if ser.in_waiting > 0:
#         command = ser.read().decode('utf-8')
#         if command == 'Run Python Script':
#             # Execute Python script using subprocess
#             subprocess.run(['python', 'circleDetect.py'])
#             for files in os.listdir("Images"):
#                 if files.endswith("bmp"):
#                     DetectCircle(files)
#             done = True
#     if done:
#         break

