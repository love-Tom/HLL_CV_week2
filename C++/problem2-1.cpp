/* @file           problem2-1.cpp
 * @author         zhaoqicheng(zhaoqicheng2022@163.com)
 * @brief          load video 
 * @details
 * @version        0.1.0
 * @date             2022-4-17
 * @copyright   2022-4-17 by zhaoqicheng.All rights reserved.
 */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

int pos = 0;
VideoCapture cap;

//回调函数
void onTrackbarSlide(int pos,void* )
{
    cap.set(CAP_PROP_POS_FRAMES,pos);   //设置当前的视频帧数与滑块位置相对应
}

int main() {
    cap.open("/home/jichuan-ftp/video/SpongeBob Squarepants.mp4");  //video
    if (!cap.isOpened()) {
        cout << "Play video failed!" << endl;
    }
    namedWindow("problem2-1", 1);
    int frames = cap.get(CAP_PROP_FRAME_COUNT);
    //将一个名为scrollbar的滑动条显示在名为problem2-1的窗口上，当滑块位置pos变化时调用回调函数onTrackbarSlide。
    createTrackbar("scrollbar", "problem2-1", &pos, frames, onTrackbarSlide); 

    while (1) {
        Mat frame;
        cap>>frame;
        if (frame.empty()) break;
        imshow("problem2-1", frame);
        char c=waitKey(30);

        if (c == 27) {  //esc退出
            break;
        }
        if(waitKey(3) == 32)  //空格键暂停
            waitKey(0);
        if(waitKey(3) == 68){  //D键快进
            for(int i=0;i<4;i++)  //快进4帧
                cap>>frame;
        }
    }
    return 0;
}
