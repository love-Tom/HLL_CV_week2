# @file		    problem2-2-1.py
# @author      	ZhaoQicheng(zhaoqicheng2022@163.com/love-Tom)
# @brief        RGB2GRAY/RGB2HSV
# @detail       自行设计算法实现RGB转GRAY功能与RGB转HSV功能，分别输出原图，灰度图和HSV图，不得调用现成函数
# @version 	    0.1.0
# @date 		2022-4-18
# @copyright	2022-4-18 by zhaoqicheng.All rights reserved. 
  
import cv2 as cv
import numpy as np

#函数定义
#RGB转GRAY函数
def RgbToGray(image):
    row, col, channel = image.shape  #读取图片的属性(行，列，通道数)
    gray = np.zeros((row, col))  #创建灰度图
    #遍历灰度图进行转换操作
    for r in range(row):
        for l in range(col):
            gray[r, l] = 0.299 * image[r, l, 0] + 0.587 * image[r, l, 1] + 0.114 * image[r, l, 2]  #转换公式
    
    cv.imshow("GRAY",gray.astype("uint8"))  #强制转换为整型
#RGB转HSV函数
def RgbToHsv(image):
    #读取RGB图像的属性
    row = image.shape[0]
    col = image.shape[1]
    #分别创建H,S,V图像
    H = np.zeros((row,col),np.float32)
    S = np.zeros((row,col),np.float32)
    V = np.zeros((row,col),np.float32)
    #分离RGB图像的三个通道
    r,g,b = cv.split(image)
    #将r,g,b转换到0-1之间
    r, g, b = r/255.0, g/255.0, b/255.0
    #遍历进行转换操作
    for i in range(0, row):
        for j in range(0, col):
            big = max((b[i, j], g[i, j], r[i, j]))
            small = min((b[i, j], g[i, j], r[i, j]))
            V[i, j] = big  #将最大值赋值给V
            if V[i, j] == 0:
                S[i, j] = 0  #最大值为0，S赋值为0
            else:
                S[i, j] = (V[i, j] - small) / V[i, j]
            if big == small:  #最大值与最小值相等，H赋0
                H[i, j] = 0  
            elif V[i, j] == r[i, j]:
                if g[i, j] >= b[i, j]:
                    H[i, j] = (60 * ((g[i, j]) - b[i, j]) / (V[i, j] - small))     #最大值等于r且g>=b
                else:
                    H[i, j] = (60 * ((g[i, j]) - b[i, j]) / (V[i, j] - small))+360 #最大值等于r且g<b
            elif V[i, j] == g[i, j]:
                H[i, j] = 60 * ((b[i, j]) - r[i, j]) / (V[i, j] - small) + 120     #最大值等于g
            elif V[i, j] == b[i, j]:
                H[i, j] = 60 * ((r[i, j]) - g[i, j]) / (V[i, j] - small) + 240     #最大值等于b
            H[i,j] = H[i,j] / 2
    #合并之前分离的H,S,V通道
    hsv = cv.merge([H,S,V])
    cv.imshow("HSV",hsv)


#函数调用
image = cv.imread("D:\photo\lena.jpg")  #读取图像
gray = RgbToGray(image)                 #生成灰度图
RgbToHsv(image)                         #生成HSV图
cv.imshow("RGB",image)                  #展示RGB图

cv.waitKey(0)
cv.destroyAllWindows()