/* @file           problem2-2-3.cpp
 * @author         zhaoqicheng(zhaoqicheng2022@163.com)
 * @brief
 * @details
 * @version        0.1.0
 * @date           2022-4-17
 * @copyright      2022-4-17 by zhaoqicheng.All rights reserved.
 */
#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>

using namespace cv;
using namespace std;

void houghlinedetect(int,void*);  //霍夫检测直线方法的定义
Mat src,roiImg,dst,gray_dst;
int threshold_value = 50;
int max_value = 255;
const char* output_title = "detect lines";

int main() {
    src = imread("/home/jichuan-ftp/image/unchanged/hough_line_detection.png",1);

    if (src.empty()) {
        cout << "图片未找到！" << endl;
        return -1;
    }

    imshow("input image",src);
    Rect roi = Rect(8, 8, src.cols - 10,src.rows - 10);  //从左上角(8,8)，选取该区域
    roiImg = src(roi);

    namedWindow(output_title);
    createTrackbar("threshold", output_title, &threshold_value, max_value, houghlinedetect);
    houghlinedetect(0, 0);
    imshow("roiImg", roiImg);

    waitKey(0);

    return 0;
}

void houghlinedetect(int, void *)
{
    //使用边缘检测将图片二值化
    Canny(roiImg, dst, threshold_value, 2 * threshold_value, 3, false);
    //bitwise_not(dst, dst, Mat());
    vector<Vec4i> lines;  //存储直线数据
    HoughLinesP(dst,lines,1, CV_PI/180.0,30,30,0);  //HoughLines需要二值图像

    cvtColor(dst, gray_dst,COLOR_GRAY2BGR);  
    for (size_t i = 0; i < lines.size(); ++i) {
        Vec4i l = lines[i];
        line(gray_dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186, 88, 255), 1, LINE_AA);
    }
    imshow(output_title, gray_dst);
}

