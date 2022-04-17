# @file         problem2-2-3.py
# @author       zhaoqicheng(zhaoqicheng2022@163.com)
# @brief            
# @details        
# @version       0.1.0
# @date         2022-4-17
# @copyright    2022-4-17 by zhaoqicheng.All rights reserved.
import cv2 as cv
import numpy as np
 
img = cv.imread('D:/photo/hough_line_detection.png')
img1 = img.copy()
img2 = img.copy()
img = cv.GaussianBlur(img, (3, 3), 0)
gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
edges = cv.Canny(gray, 50, 150, apertureSize=3)
lines = cv.HoughLines(edges, 1, np.pi/180, 110)
 
for line in lines:
    rho = line[0][0]
    theta = line[0][1]
    a = np.cos(theta)
    b = np.sin(theta)
    x0 = a*rho
    y0 = b*rho
    x1 = int(x0 + 1000*(-b))
    y1 = int(y0 + 1000*(a))
    x2 = int(x0 - 1000*(-b))
    y2 = int(y0 - 1000*(a))
 
    cv.line(img1, (x1, y1), (x2, y2), (0, 0, 255), 2)
 
lines = cv.HoughLinesP(edges, 1, np.pi/180, 30, 300, 5)
 
for line in lines:
    x1 = line[0][0]
    y1 = line[0][1]
    x2 = line[0][2]
    y2 = line[0][3]
    cv.line(img2, (x1, y1), (x2, y2), (0, 255, 0), 2)
 
cv.imshow('houghlines3', img1)
cv.imshow('edges', img2)
cv.waitKey(0)
print(lines)
