# @file		    problem2-2-2.py
# @author      	ZhaoQicheng(zhaoqicheng2022@163.com/love-Tom)
# @brief        
# @detail       
# @version 	    0.1.0
# @date 		2022-4-18
# @copyright	2022-4-18 by zhaoqicheng.All rights reserved. 
  
import numpy as np
import cv2 as cv 

#定义高斯滤波函数
def GaussianFilter(image, K_size=3, sigma=2): #设置卷积核，方差，不设置会自动生成，默认值分别为3*3，2
    #读出图片属性(长宽高)
    H, W, C = image.shape    
    
    #计算需要填充的0层，并填充
    pad = K_size // 2
    image_changed = np.zeros((H + pad * 2, W + pad * 2, C), dtype=np.float64)  #填充后的image_changed
    image_changed[pad: pad + H, pad: pad + W] = image.copy().astype(np.float64)
    
    #卷积核设置
    K = np.zeros((K_size, K_size), dtype=np.float64)   #初始化
    
    #遍历进行操作
    for x in range(-pad, -pad + K_size):
        for y in range(-pad, -pad + K_size):
            K[y + pad, x + pad] = np.exp( -(x ** 2 + y ** 2) / (2 * (sigma ** 2)))  #二维高斯分布中的指数部分
    K /= (2 * np.pi * sigma * sigma)    #二维高斯分布的系数
    K /= K.sum()
    tmp = image_changed.copy()  #拷贝一份用来操作
    
    #遍历进行卷积运算
    for y in range(H):
        for x in range(W):
            for c in range(C):
                image_changed[pad + y, pad + x, c] = np.sum(K * tmp[y: y + K_size, x: x + K_size, c])#先乘后加
    image_changed = np.clip(image_changed, 0, 255)
    image_changed = image_changed[pad: pad + H, pad: pad + W].astype(np.uint8)   #数据类型转换
    return image_changed

#读取图像
image = cv.imread("D:/photo/lena.jpg")
#对图像进行处理
image_changed = GaussianFilter(image, 3, 2) 
cv.imshow("image",image)
cv.imshow("image_changed",image_changed)      

cv.waitKey(0)
cv.destroyAllWindows()