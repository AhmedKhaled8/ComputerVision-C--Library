import numpy as np
from numpy import fft
import cv2
from matplotlib.pyplot import plot

img = cv2.imread("/home/primary/git/CV/ComputerVision-Task1/fft/cheekcell.jpg",0)

fourier = fft.fft2(img)

lowpass = fourier.copy()
highpass = fourier.copy()

low_lrange = 64 - 40
low_rrange = 64 + 40

high_lrange = 50
high_rrange = 128 - 50

lowpass[20:108,20:108] = 0
highpass[0:high_lrange] = 0
highpass[high_rrange:128] = 0
highpass[:,0:high_lrange] = 0
highpass[:,high_rrange:128] = 0

original_image = np.uint8(np.real(fft.ifft2(fourier)))

low_image = np.uint8(np.real(fft.ifft2(lowpass)))
high_image = np.uint8(np.real(fft.ifft2(highpass)))

# cv2.namedWindow("Image")
# cv2.imshow("Image", original_image)
# cv2.namedWindow("Low pass")
# cv2.imshow("Low pass",low_image)
# cv2.namedWindow("High pass")
# cv2.imshow("High pass",high_image)
# cv2.waitKey()
# cv2.destroyAllWindows()

cv2.imwrite("low_pass.jpg", low_image)
cv2.imwrite("high pass.jpg", high_image)
