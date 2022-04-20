/* @file               problem2-1.cpp
 * @author         zhaoqicheng(zhaoqicheng2022@163.com)
 * @brief
 * @details
 * @version        0.1.0
 * @date             2022-4-19
 * @copyright   2022-4-19 by zhaoqicheng.All rights reserved.
 */
#include <opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

int g_slider_position = 0;//全局变量，用于creatTrackbar()函数中的第三个参数,初始化阈值
VideoCapture g_cap;//创建VideoCapture类对象g_cap
//回调函数
void onTrackbarSlide(int pos, void*) {
    g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);//设置属性
}

int main() {
    namedWindow("pipihan21", WINDOW_NORMAL);//创建窗口
    g_cap.open("/home/jichuan-ftp/video/SpongeBob Squarepants.mp4");//读取视频
    int frames = (int)g_cap.get(CAP_PROP_FRAME_COUNT);//得到视频总帧数
    int tmpw = (int)g_cap.get(CAP_PROP_FRAME_WIDTH);//得到视频每一帧的宽度
    int tmph = (int)g_cap.get(CAP_PROP_FRAME_HEIGHT);//得到视频每一帧的高度
    cout << "Video has" << frames << "frames of dimensions(" << tmpw << "," << tmph << ")." << endl;
    createTrackbar("滑动条", "pipihan21", &g_slider_position, frames, onTrackbarSlide);//creatTrackbar()函数实现滑动条的创建
    Mat frame;//定义一个Mat类对象frame，用于储存每一帧图像
    while (1) {
            g_cap >> frame;//读取当前帧
            if (frame.empty())//判断当前帧是否为空
                break;
            int current_pos = (int)g_cap.get(CAP_PROP_POS_FRAMES);//获取滑条的当前位置
            setTrackbarPos("滑动条", "pipihan21", current_pos);//设置滑动条的新位置，从而实现滑动条的跟随
            imshow("pipihan21", frame);//显示图像

        char c = (char)cv::waitKey(33);//若33毫秒内按下键，字符c储存该键
        //按键实现相关功能
        if (c == 27)  {//“Esc”的ASKII为27
            destroyWindow("pipihan21");//销毁窗口
            cout << "成功销毁窗口" << endl;
            break;
        }
        if(c == 32) { //空格暂停以及继续播放
            waitKey(0);
            if(c == 32) {
                continue;
            }
        }
        if(c == 100) {  //d键前进4帧
            current_pos+=4;
            setTrackbarPos("滑动条", "pipihan21", current_pos);//设置滑动条的新位置，从而实现滑动条的跟随
            imshow("pipihan21", frame);//显示图像
            continue;
        }
        if(c == 97) {  //a键后退四帧
            current_pos-=4;
            setTrackbarPos("滑动条", "pipihan21", current_pos);//设置滑动条的新位置，从而实现滑动条的跟随
            imshow("pipihan21", frame);//显示图像
            continue;
        }
    }
    destroyWindow("pipihan21");
    cout << "成功销毁窗口" << endl;
    return 0;
}
