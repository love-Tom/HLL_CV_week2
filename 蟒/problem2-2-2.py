import cv2
 
img=cv2.imread("D:/photo/lena.jpg")
out=cv2.GaussianBlur(img,(3,3),2,0)
cv2.imshow('result',out)
cv2.waitKey(0)
cv2.destroyAllWindows()
 