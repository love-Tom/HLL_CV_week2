/* @file                problem2-1-1.cpp
 * @author          zhaoqicheng(zhaoqicheng2022@163.com)
 * @brief              BGR2GRAY and BGR2HSV
 * @details
 * @version         0.1.0
 * @date              2022-4-12
 * @copyright    2022-4-12 by zhaoqicheng.All rights reserved.
 */
//RGB转HSV

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void RgbToHsv(Mat image_rgb, Mat &image_hsv)
{
  //创建一张三通道的HSV图像
  image_hsv = Mat::zeros(image_rgb.size(), CV_8UC3);
  //遍历实现操作
  for (int i = 0; i < image_rgb.rows; i++)
  {
    Vec3b *p0 = image_rgb.ptr<Vec3b>(i);   //B=p[0]  G=p[1]  R=p[2]
    Vec3b *p1 = image_hsv.ptr<Vec3b>(i);   //B=p[0]  G=p[1]  R=p[2]

    for (int j = 0; j < image_rgb.cols; j++)
    {
      //将图像任意坐标点的BGR分别转换到0-1之间
      float B = p0[j][0] / 255.0;
      float G = p0[j][1] / 255.0;
      float R = p0[j][2] / 255.0;

      float V = (float)std::max({ B, G, R });     //B/G/R
      float vmin = (float)std::min({ B, G, R });
      float diff = V - vmin;

      float S, H;
      S = diff / (float)(fabs(V) + FLT_EPSILON);
      diff = (float)(60.0 / (diff + FLT_EPSILON));

      if (V == B) { //V=B
        H = 240.0 + (R - G) * diff;
      }
      else if (V == G) {  //V=G
              H = 120.0 + (B - R) * diff;
      }
      else if (V == R) {  //V=R
        H = (G - B) * diff;
      }
     //如果如果计算得到的H值小于0，将该值再加上360，得到最终的H值
      H = (H < 0.0) ? (H + 360.0) : H;

      p1[j][0] = (uchar)(H / 2);
      p1[j][1] = (uchar)(S * 255);
      p1[j][2] = (uchar)(V * 255);
    }
  }
}

//RGB转GRAY
void RgbToGray(const Mat &image_rgb, Mat &image_gray) {
    if (!image_rgb.data || image_rgb.channels() != 3) {
        return;
    }
    //创建一张单通道的灰度图像
    image_gray = Mat::zeros(image_rgb.size(), CV_8UC1);
    //取出存储图像像素的数组的指针
    uchar *pointImage = image_rgb.data;
    uchar *pointImageGray = image_gray.data;
    //取出图像每行所占的字节数
    size_t stepImage = image_rgb.step;
    size_t stepImageGray = image_gray.step;
    for (int i = 0; i < image_gray.rows; i++) {
        for (int j = 0; j < image_gray.cols; j++) {
            pointImageGray[i*stepImageGray + j] =
                           (uchar)(0.114*pointImage[i*stepImage + 3 * j] + 0.587*pointImage[i*stepImage + 3 * j + 1]+0.299*pointImage[i*stepImage + 3 * j + 2]);
        }
    }
}


int main() {
    Mat image = imread("/home/jichuan-ftp/image/unchanged/lena.jpg");
    Mat gray,hsv;
    RgbToHsv(image, hsv);
    RgbToGray(image, gray) ;
    imshow("RGB",image);
    imshow("GRAY",gray);
    imshow("HSV",hsv);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
