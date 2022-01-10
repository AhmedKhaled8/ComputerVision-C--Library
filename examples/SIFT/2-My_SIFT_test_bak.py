import numpy as np
import cv2

sigma = 1.6
k = 1.414
num_octaves = 4
num_scales = 5
edgeThreshold = 10
contrastThreshold = 8

img = cv2.imread("Eiffel_1.png")
# img = cv2.imread("feature_building.jpg")

gray= cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

# Get octaves

scales = [[] for i in range(num_octaves)]
scales[0].append(gray)

for index in range(1, num_octaves):
    resized = None
    resized = cv2.resize(scales[index - 1][0],
                         (scales[index - 1][0].shape[0] // 2, scales[index - 1][0].shape[1] // 2))
    scales[index].append(resized)

#________________________________

# Get scales
for index1 in range(0, num_octaves):
    blurred = None
    for index2 in range(1, num_scales):
        blurred = cv2.GaussianBlur(scales[index1][index2 - 1],ksize = (0,0), sigmaX = (k ** index2) * sigma)
        scales[index1].append(blurred)

#________________________________

# Get difference of Gaussian
DoG = [[] for i in range(num_octaves)]

for index1 in range(0, num_octaves):
    for index2 in range(0, num_scales - 1):
        DoG[index1].append(scales[index1][index2] - scales[index1][index2 + 1])

#________________________________

# Get local extrema
# -1 for the difference between num_scales and length of DoG
maximas = []
minimas = []
for index1 in range(0, num_scales - 1):
    for x in range(scales[index1][0].shape[0] - 2):
        for y in range(scales[index1][0].shape[1] - 2):
            maxima = 0
            minima = 0
            max_index = (x,y,0)
            min_index = (x,y,0)
            for scale_in in range(3):
                for x_in in range(3):
                    for y_in in range(3):
                        calc = int(scales[index1][scale_in][x,y]) - int(scales[index1][scale_in][x + x_in, y + y_in])
                        if(calc == 0):
                            continue
                        if(calc > minima):
                            minima = calc
                            min_index = (x + x_in, y + y_in, scale_in)
                        if(calc < maxima):
                            maxima = calc
                            max_index = (x + x_in, y + y_in, scale_in)
            maximas.append(max_index)
            minimas.append(min_index)


contrast_maximas = []
contrast_minimas = []
loopers = [minimas, maximas]
contrast_loopers = [contrast_maximas, contrast_minimas]

#TODO move this loop in the other loop

for i in range(2):
    for x,y,s in loopers[i]:
        if x + 1 >= scales[0][s].shape[0] or y + 1 >= scales[0][s].shape[1] or s +1 >= len(scales[0]):
            continue
        if x - 1 < 0 or y - 1 < 0 or s - 1 < 0:
            continue
        dx = (float(scales[0][s][x + 1, y]) - scales[0][s][x - 1, y]) / 2
        dy = (float(scales[0][s][x, y + 1]) - scales[0][s][x, y - 1]) / 2
        ds = (float(scales[0][s + 1][x, y]) - scales[0][s - 1][x, y]) / 2
        
        
        D = np.array([dx, dy, ds])
        h = np.array((x,y,s))
        
        #______________________________________________________
        
        dxx = (float(scales[0][s][x + 1, y]) - 2 * scales[0][s][x, y] + scales[0][s][x -1, y]) / 3
        dyy = (float(scales[0][s][x, y + 1]) - 2 * scales[0][s][x, y] + scales[0][s][x, y - 1]) / 3
        dss = (float(scales[0][s + 1][x, y]) - 2 * scales[0][s][x, y] + scales[0][s - 1][x, y]) / 3
        
        calc1 = (float(scales[0][s][x + 1, y + 1]) + scales[0][s][x - 1, y - 1])
        calc2 = (float(scales[0][s][x + 1, y - 1]) + scales[0][s][x - 1, y + 1])
        dxy = (calc1 - calc2) / 4
        
        calc1 = (float(scales[0][s + 1][x + 1, y]) + scales[0][s - 1][x - 1, y])
        calc2 = (float(scales[0][s - 1][x + 1, y]) + scales[0][s + 1][x - 1, y])
        dxs = (calc1 - calc2) / 4
        
        calc1 = (float(scales[0][s + 1][x, y + 1]) + scales[0][s - 1][x, y - 1])
        calc2 = (float(scales[0][s - 1][x, y + 1]) + scales[0][s + 1][x, y - 1])
        dys = (calc1 - calc2) / 4
        
        H = np.array ([[dxx, dxy, dxs], [dxy, dyy, dys], [dxs, dys, dss]])
        Tr_sq = (dxx + dyy) * (dxx + dyy)
        Det = dxx * dyy - dxy * dxy
        if Det == 0:
            continue
        
        Threshval = float(scales[0][s][x,y]) + np.matmul(D, (x,y,s))  + 0.5 * np.matmul(np.matmul(h.transpose(),H), h)
        
        edgebool = Tr_sq / Det > (edgeThreshold + 1) * (edgeThreshold + 1) / edgeThreshold
        contrastbool = abs(Threshval) > contrastThreshold
        
        if (edgebool and contrastbool):
            contrast_loopers[i].append((x,y,s))


hist = np.zeros((36), dtype = np.float32)

features = []

for i in range(2):
    for x,y,s in contrast_loopers[i]:
        hist[:] = 0
        sig = (k ** s) * sigma
        xcounter = -1
        
        ksize = int(((sig - 0.8) / 0.3 + 1) * 2 + 1)
        
        if ksize % 2 == 0:
            ksize += 1
        
        offset = ksize // 2
        
        gaussian_window = np.zeros((ksize,ksize), dtype = np.float32)
        
        gaussian_window[6,6] = 1
        
        gaussian_window = cv2.GaussianBlur(gaussian_window, (ksize,ksize), -1)
        
        
        for x_sub in range(x - offset, x + offset + 1):
            ycounter = -1
            xcounter += 1
            if x_sub  - 1 < 0:
                continue
            if x_sub + 1 >= scales[0][s].shape[0]:
                continue
            for y_sub in range(y - offset, y + offset + 1):
                ycounter += 1
                if y_sub - 1 < 0:
                    continue
                if y_sub + 1 >= scales[0][s].shape[1]:
                    continue
                term1 = float(scales[0][s][x_sub + 1, y_sub]) - scales[0][s][x_sub - 1,y_sub]
                term2 = float(scales[0][s][x_sub, y_sub + 1]) - scales[0][s][x_sub,y_sub - 1]
                mag = np.sqrt(term1 * term1 + term2 * term2)
                
                if term1 == 0:
                    theta = 1.5708
                else:
                    theta = np.arctan(term2 / term1)
                
                theta = theta * 180 / 3.14159
                
                if theta < 0:
                    theta += 360
                
                theta = int(theta // 10)
                
                hist[theta] += mag * gaussian_window[xcounter, ycounter]
        max_hist = np.max(hist)
        
        for val in hist:
            if val > 0.8 * max_hist:
                features.append((x,y,s,theta))

# _________________________________________________

full_features = []

for x,y,s, angle in features:
    feature_desc = [0 for _ in range(128)]
    hist[:] = 0
    xcounter = -1
    
    offset = 8
    sig = offset
    
    if x - offset < 0 or y - offset < 0:
        continue
    
    if x + offset > scales[0][s].shape[0] or y + offset > scales[0][s].shape[1]:
        continue
    
    sub_sample = scales[0][s][x - offset : x + offset , y - offset : y + offset]
    
    sample_gauss = cv2.GaussianBlur(sub_sample, (0,0), sig)
    
    for index2 in range(0, 2 * offset, 4):
        for x_in in range(0, 4):
            for y_in in range(0,4):
                term1 = float(scales[0][s][x_in + 1, y_in]) - scales[0][s][x_in - 1,y_in]
                term2 = float(scales[0][s][x_in, y_in + 1]) - scales[0][s][x_in,y_in - 1]
                mag = np.sqrt(term1 * term1 + term2 * term2)
                
                if term1 == 0:
                    theta = 1.5708
                else:
                    theta = np.arctan(term2 / term1)
                
                theta = theta * 180 / 3.14159
                
                theta -= angle * 10
                
                if theta < 0:
                    theta += 360
                
                # Note: theta is divided here by 45 not 10 as previously
                theta = int(theta // 45)
                
                feature_desc[index2 * theta + theta] += mag * sample_gauss[index2 + x_in, index2 + y_in]
    
    feature_desc = np.array(feature_desc)
            
    summer = np.sum(feature_desc * feature_desc)
    root_sum = np.sqrt(summer)
    feature_desc /= root_sum
            
    # Achieve illumination invariance
            
    for j in range(128):
        if feature_desc[j] > 0.2:
            feature_desc[j] = 0.2
    
    summer = np.sum(feature_desc * feature_desc)
    root_sum = np.sqrt(summer)
    feature_desc /= root_sum
                
    full_features.append((x,y,s,angle, feature_desc))
          

for x,y,s
    
"""
cv2.namedWindow("Photo")
cv2.imshow("Photo", DoG[0][2])
cv2.waitKey(0)
cv2.destroyAllWindows()
"""
