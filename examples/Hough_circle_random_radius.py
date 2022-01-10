import numpy as np
import cv2

# original_img = cv2.imread("/home/primary/git/CV/ComputerVision-Task2/imgs/Hough_circle.jpg", 0)
original_img = cv2.imread("/home/primary/git/CV/ComputerVision-Task2/imgs/Simple_circle_r45.png", 0)
# original_img = cv2.imread("/home/primary/git/CV/ComputerVision-Task2/imgs/Hough_two_circle.png", 0)

img = cv2.Canny(original_img, 50, 80)

color_photo = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)

num_thetas = 100

thetas = np.deg2rad(np.linspace(0, 360, num_thetas, endpoint= False))
thetas_circle = np.deg2rad(np.linspace(0, 360, 360, endpoint = False))

width, height = img.shape

diag_len = np.int(np.sqrt(width * width + height * height)) + 1

num_rhos = np.min((width, height)) // 10

rhos = np.linspace(10,np.min((width, height)) // 2, num_rhos, endpoint = False, dtype = np.uint16)

cos_t = np.cos(thetas)
sin_t = np.sin(thetas)

cos_circle = np.cos(thetas_circle)
sin_circle = np.sin(thetas_circle)

acc_size = 2 * diag_len + 1

accumulator = np.zeros((acc_size, acc_size), dtype=np.uint16)

y_idxs, x_idxs = np.nonzero(img)
# rhos = [50]
for r in rhos:
    rhoMULcos = r * cos_t
    rhoMULsin = r * sin_t
    print(r)
    for i in range(x_idxs.shape[0]):
        x = x_idxs[i]
        y = y_idxs[i]
        
        for t_idx in range(num_thetas):
                # diag_len is added for a positive index
                a = int (x - rhoMULcos[t_idx] + diag_len)
                b = int (y - rhoMULsin[t_idx] + diag_len)
                if (a >= acc_size or b >= acc_size): continue
                accumulator[a, b] += 1
    
    threshold = np.max(accumulator) - 1
    if threshold < 20:
        print("No circles")
        continue
    idx = np.where(accumulator > threshold)
    a = idx[0]
    b = idx[1]
    
    for a_idx in range(a.shape[0]):
        y = np.uint32(a[a_idx] + r * sin_circle - diag_len)
        x = np.uint32(a[a_idx] + r * cos_circle - diag_len)
        for idx in range(x.shape[0]):
            if x[idx] >= width or y[idx] >= height: continue
            if x[idx] < 0 or y[idx] < 0: continue
            color_photo[x[idx], y[idx]] = [255,0,0]        


cv2.namedWindow("Quantized")
cv2.imshow("Quantized",color_photo)
cv2.waitKey()
cv2.destroyAllWindows()

