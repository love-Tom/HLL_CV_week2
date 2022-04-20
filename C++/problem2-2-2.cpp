/* @file                problem2-2-2.cpp
 * @author         zhaoqicheng(zhaoqicheng2022@163.com)
 * @brief             z
 * @details         z
 * @version        0.1.0
 * @date             2022-4-11
 * @copyright   2022-4-11 by zhaoqicheng.All rights reserved.
 */

#include<opencv2/opencv.hpp>
#include<iostream>
#include<opencv2/core.hpp>
#include<cmath>
#include<opencv2/highgui.hpp>
using namespace std;
using namespace cv;

//函数声明
void listPixel_Operate(int, void*);
void Gaussian(const Mat &input, Mat &output, int a,int b,double sigma);

int sig;
int a,b;
Mat image;

int main() {
    cout << "请输入sigma的值：" << endl;
    cin >> sig;
    sig = 2;
    cout<<"请分别输入卷积核行列"<<endl;
    cin>>a;
    cin>>b;
    image = imread("/home/jichuan-ftp/image/unchanged/lena.jpg");
    namedWindow("【效果图窗口】", 1);
    //调用回调函数
    listPixel_Operate(sig, 0);
    //system("pause");
    waitKey(0);
    return 0;
}
、
//函数声明
void Gaussian(const Mat &input, Mat &output, int a,int b,double sigma) {
    int row = input.rows, col = input.cols;
   // int window = (int)((6 * sigma - 1) / 2) * 2 + 1;//滤波窗口
    int window = a*b;
    double *temp = new double[window];

    //扩充边界
    Mat INPUT;
    copyMakeBorder(input, INPUT, window / 2, window / 2, window / 2, window / 2, BORDER_REFLECT_101);

    double sum = 0;
    for (int w = 0; w < window; w++) {
        int mid = w - window / 2;
        temp[w] = exp(-(mid*mid) / (2 * sigma*sigma));
        sum += temp[w];
    }

    //归一化滤波核，和为1
    for (int w = 0; w < window; w++)
        temp[w] = temp[w] / sum;

    //扩充边界之后的长宽
    int rows = row + window - 1;
    int cols = col + window - 1;

    //先对每行进行一维高斯滤波
    for (int y = window / 2; y < row + window / 2; y++)//行
    {
        for (int x = window / 2; x < col + window / 2; x++) //列
        {
            int num = 0;
            double pix[3] = { 0 };
            for (int k = x - window / 2; k < x + window / 2; k++)
            {
                for (int c = 0; c < INPUT.channels(); c++)
                {
                    pix[c] += (INPUT.at<Vec3b>(y, k)[c])*temp[num];   //列坐标<矩阵列数
                }
                num++;
            }
            for (int c = 0; c < INPUT.channels(); c++)
            {
                INPUT.at<Vec3b>(y, x)[c] = pix[c];
            }
        }
    }

    //再对每列进行一维高斯滤波
    for (int x = window / 2; x < col + window / 2; x++) {
        for (int y = window / 2; y < row + window / 2; y++) {
            int num = 0;
            double pix[3] = { 0 };
            for (int k = y - window / 2; k < y + window / 2; k++) {
                for (int c = 0; c < INPUT.channels(); c++)
                    pix[c] += (INPUT.at<Vec3b>(k, x)[c])*temp[num];
                num++;
            }
            for (int c = 0; c < INPUT.channels(); c++)
                INPUT.at<Vec3b>(y, x)[c] = pix[c];
        }
    }
    for (int y = 0; y < row; y++) {
        for (int x = 0; x < col; x++)  {
            output.at<Vec3b>(y, x) = INPUT.at<Vec3b>(y + window / 2, x + window / 2);
        }
    }
}


void listPixel_Operate(int, void*) {
    Mat dst = Mat::zeros(image.rows, image.cols, image.type());
    imshow("原图窗口", image);
    Gaussian(image, dst,a,b,sig);
    imshow("【效果图窗口】", dst);
}

