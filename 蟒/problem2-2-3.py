# @file		    problem2-2-3.py
# @author      	ZhaoQicheng(zhaoqicheng2022@163.com/love-Tom)
# @brief        
# @detail       
# @version 	    0.1.0
# @date 		2022-4-18
# @copyright	2022-4-18 by zhaoqicheng.All rights reserved. 
  
import cv2 as cv
import numpy as np
img = cv.imread("D:/photo/hough_line_detection.png")


def line_detection(image):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    # apertureSize做Canny时梯度窗口的大小
    edges = cv.Canny(gray, 50, 150, apertureSize=3)
    # 返回的是r和theta
    lines = cv.HoughLines(edges, 1, np.pi/180, 200)
    for line in lines:
        print(type(line))
        rho, theta = line[0]
        a = np.cos(theta)
        b = np.sin(theta)
        x0 = a*rho
        y0 = b*rho
        # 乘以1000，是根据源码乘的，通过x1、x2、y1、y2画一条直线
        x1 = int(x0+1000*(-b))
        y1 = int(y0+1000*a)
        x2 = int(x0-1000*(-b))
        y2 = int(y0-1000*a)
        cv.line(image, (x1, y1), (x2, y2), (0, 0, 255), 2)  # 2是所画直线长度的宽
        print(x1,y1,x2,y2)
    cv.imshow('image_lines', image)

line_detection(img);
cv.waitKey(0)
cv.destroyAllWindows()

