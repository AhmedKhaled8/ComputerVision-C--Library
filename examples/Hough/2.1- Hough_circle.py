import numpy as np
import cv2

# original_img = cv2.imread("/home/primary/git/CV/ComputerVision-Task2/imgs/Hough_circle.jpg", 0)
original_img = cv2.imread("/home/primary/git/CV/ComputerVision-Task2/imgs/Simple_circle_r45.png", 0)

img = cv2.Canny(original_img, 50, 80)

num_thetas = 30

thetas = np.deg2rad(np.linspace(0, 90, num_thetas ))
# thetas = np.deg2rad(np.linspace(0, 350, num_thetas))
thetas_circle = np.deg2rad(np.linspace(0, 360, 360, endpoint = False))

width, height = img.shape

diag_len = np.int(np.sqrt(width * width + height * height)) + 1

rhos = np.linspace(0, diag_len , 40, dtype = np.int16)

cos_t = np.cos(thetas)
sin_t = np.sin(thetas)

cos_circle = np.cos(thetas_circle)
sin_circle = np.sin(thetas_circle)

acc_size = 2 * diag_len + 1

accumulator = np.zeros((acc_size, acc_size), dtype=np.uint16)

y_idxs, x_idxs = np.nonzero(img)

r = 45

for i in range(x_idxs.shape[0]):
    x = x_idxs[i]
    y = y_idxs[i]
    
    for t_idx in range(num_thetas):
            # diag_len is added for a positive index
            a = int (x - r * cos_t[t_idx] + diag_len)
            b = int (y - r * sin_t[t_idx] + diag_len)
            if (a >= acc_size or b >= acc_size): continue
            accumulator[a, b] += 1

threshold = np.max(accumulator) - 1
if threshold < 10:
    print("No circles")
idx = np.where(accumulator > threshold)
a = idx[0]
b = idx[1]

color_photo = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)

for a_idx in range(a.shape[0]):
    x = np.uint16(a[a_idx] + r * cos_circle - diag_len - 1)
    y = np.uint16(b[a_idx] + r * sin_circle - diag_len - 1)
    for idx in range(x.shape[0]):
        color_photo[x[idx], y[idx]] = [255,0,0]        


cv2.namedWindow("Quantized")
cv2.imshow("Quantized",color_photo)
cv2.waitKey()
cv2.destroyAllWindows()

