/* @file           problem2-2-2.cpp
 * @author         zhaoqicheng(zhaoqicheng2022@163.com)
 * @brief           
 * @details         
 * @version        0.1.0
 * @date           2022-4-13
 * @copyright      2022-4-13 by zhaoqicheng.All rights reserved.
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//滤波 = 卷积（ 输入信号 ，卷积模板 ）
//函数声明
void GaussianBlur(Mat& image) ;

int main() {
    Mat image = imread("/home/jichuan-ftp/image/unchanged/lena.jpg");
    GaussianBlur(image);

    return 0;
}

void GaussianBlur(Mat &image) {
    Mat dst;
    GaussianBlur(image,dst,Size(3,3),2,0);
    imshow("gaussian_lena",dst);

    waitKey(0);
    destroyAllWindows();
}
