/* @file            problem2-1-1.cpp
 * @author          zhaoqicheng(zhaoqicheng2022@163.com)
 * @brief           BGR2GRAY and BGR2HSV
 * @details
 * @version         0.1.0
 * @date            2022-4-12
 * @copyright       2022-4-12 by zhaoqicheng.All rights reserved.
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    //读取图像
    Mat image = imread("/home/jichuan-ftp/image/unchanged/lena.jpg");
    //创建GRAY图和HSV图
    Mat gray,hsv;

    //判断非空
    if (image.empty()) {
            cout<<"can not read this picture !"<<endl;
            return -1;
        }

    //色彩空间转换
    cvtColor(image,gray,COLOR_BGR2GRAY);
    cvtColor(image,hsv,COLOR_BGR2HSV);

    //显示原图像
    imshow("BGR",image);
    //显示改变后的图像
    imshow("gray",gray);
    imshow("hsv",hsv);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
