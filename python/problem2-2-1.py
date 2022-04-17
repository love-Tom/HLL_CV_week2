# @file         problem2-2-1.py
# @author       zhaoqicheng(zhaoqicheng2022@163.com)
# @brief            
# @details        
# @version       0.1.0
# @date         2022-4-17
# @copyright    2022-4-17 by zhaoqicheng.All rights reserved.
import numpy as np
import cv2 as cv

#lena图片路径
lena = "D:/photo/lena.jpg"

#读取图像
photo = cv.imread(lena)

#窗口名称命名
windowgray = 'gray'
windowhsv = 'hsv'

#BGR转GRAY
lena_gray = cv.cvtColor(photo, cv.COLOR_BGR2GRAY)
#BGR转HSV
lena_hsv = cv.cvtColor(photo, cv.COLOR_BGR2HSV)

#展示图片
cv.imshow(lena, photo)
cv.imshow(windowgray, lena_gray)
cv.imshow(windowhsv, lena_hsv)

cv.waitKey(0)
cv.destoryAllWindows()

