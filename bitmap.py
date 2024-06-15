import cv2
image = cv2.imread('Images/gear.png')

h, w = image.shape[:2]

arr = ""

for i in range(0, h, 6):
    for j in range(0, w, 6):
        bit = image[i,j][0]
        if bit==0:
            arr += "1"
        else:
            arr += "0"
    arr+="n"

print(arr)
