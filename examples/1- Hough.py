import numpy as np
import cv2

# original_img = cv2.imread("/home/primary/Downloads/Hough_line.png", 0)
original_img = cv2.imread("/home/primary/Downloads/sqr1.png", 0)

img = cv2.Canny(original_img, 50, 80)

thetas = np.deg2rad(np.arange(-90, 90))
width, height = img.shape

diag_len = np.int(np.sqrt(width * width + height * height)) + 1

rhos = np.arange(-diag_len, diag_len,step = 1, dtype = np.int16)

cos_t = np.cos(thetas)
sin_t = np.sin(thetas)

num_thetas = thetas.shape[0]

accumulator = np.zeros((2 * diag_len, num_thetas), dtype=np.uint32)

y_idxs, x_idxs = np.nonzero(img)

for i in range(x_idxs.shape[0]):
    x = x_idxs[i]
    y = y_idxs[i]
    
    for t_idx in range(num_thetas):
        # diag_len is added for a positive index
        rho = round(x * cos_t[t_idx] + y * sin_t[t_idx]) + diag_len
        rho = np.int(rho)
        accumulator[rho, t_idx] += 1

threshold = np.max(accumulator) // 2
idx = np.where(accumulator > threshold)
rho = rhos[idx[0]]
theta = thetas[idx[1]]
        
mapping = accumulator * 255 / np.max(accumulator)
mapping = np.uint8(mapping)

color_photo = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)

for rh in range(0, rho.shape[0]):
    if np.isclose(np.sin(theta[rh]) ,0):
        for x in range(0, img.shape[0]):
            y = np.int(np.abs(rho[rh]))
            color_photo[x,y] = [255,0,0]
    elif np.isclose(np.cos(theta[rh]) , 0):
        for y in range(0, img.shape[1]):
            x = np.int(np.abs(rho[rh]))
            color_photo[x,y] = [255,0,0]
    else:
        for x in range(0, img.shape[0]):
            y = np.int((rho[rh] - x * np.cos(theta[rh])) / np.sin(theta[rh]))
            if abs(y) >= color_photo.shape[1]:
                continue
            color_photo[x,y] = [255,0,0]
    

cv2.namedWindow("Quantized")
cv2.imshow("Quantized",color_photo)
cv2.waitKey()
cv2.destroyAllWindows()

