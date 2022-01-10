import cv2 as cv

# img = cv.imread("Eiffel_1.png")
img = cv.imread("feature_building.jpg")

gray= cv.cvtColor(img,cv.COLOR_BGR2GRAY)

sift = cv.SIFT_create()

kp = sift.detect(gray,None)

img=cv.drawKeypoints(gray,kp,img)

cv.imwrite('OpenCV_sift_keypoints.jpg',img)