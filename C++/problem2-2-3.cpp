/* @file               problem2-2-3.cpp
 * @author         zhaoqicheng(zhaoqicheng2022@163.com)
 * @brief
 * @details
 * @version        0.1.0
 * @date             2022-4-19
 * @copyright   2022-4-19 by zhaoqicheng.All rights reserved.
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>

using namespace std;/* @file               problem2-2-3.cpp
 * @author         zhaoqicheng(zhaoqicheng2022@163.com)
 * @brief          自行设计算法实现霍夫直线检测
 * @details
 * @version        0.1.0
 * @date             2022-4-19
 * @copyright   2022-4-19 by zhaoqicheng.All rights reserved.
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>

using namespace std;
using namespace cv;

const double PI=4.0*atan(1.0); //圆周率π赋值
const double RADIAN = 180.0 / PI;

//直线数据
struct line {
    int theta;
    int r;
};

//函数声明
void ConvertRGBToGRAY(const Mat &image,Mat &imageGray);//灰度转换函数
void GetGaussianKernel(double **gaus, const int size,const double sigma);//高斯卷积核生成函数
void GaussianFilter(const Mat imageSource,Mat &imageGaussian,double **gaus,int size);//高斯滤波
void SobelGradDirction(const Mat imageSource,Mat &imageSobelX,Mat &imageSobelY,double *&pointDrection);//计算梯度和方向
void SobelAmplitude(const Mat imageGradX,const Mat imageGradY,Mat &SobelAmpXY);//计算Sobel的X和Y方向梯度幅值
void LocalMaxValue(const Mat imageInput,Mat &imageOutput,double *pointDrection);//局部极大值抑制
void DoubleThreshold(Mat &imageIput,double lowThreshold,double highThreshold);//双阈值处理
void DoubleThresholdLink(Mat &imageInput,double lowThreshold,double highThreshold);//双阈值中间像素连接处理
void drawLine(Mat &img,vector<Vec2f>lines,Scalar scalar,int n) ;//

//全局变量定义
Mat imageSource;
Mat imageGray;
Mat imageGaussian;

int main()
{
     //读入RGB图像
    imageSource=imread("/home/jichuan-ftp/image/unchanged/hough_line_detection.png");
    imshow("RGB Image",imageSource);

    //RGB转换为灰度图
    ConvertRGBToGRAY(imageSource,imageGray);
    int size=3; //定义卷积核大小
    double **gaus=new double *[size];  //卷积核数组
    for(int i=0;i<size;++i)
        gaus[i]=new double[size];  //动态生成矩阵
    GetGaussianKernel(gaus,3,2); //生成3*3 大小高斯卷积核，Sigma=2；
    imageGaussian=Mat::zeros(imageGray.size(),CV_8UC1);

     //高斯滤波
    GaussianFilter(imageGray,imageGaussian,gaus,3);
    imshow("Gaussian_Image",imageGaussian);
    Mat imageSobelY;
    Mat imageSobelX;

    //定义梯度方向角数组
    double *pointDirection=new double[(imageSobelX.cols-1)*(imageSobelX.rows-1)];

    //计算X、Y方向梯度和方向角
    SobelGradDirction(imageGaussian,imageSobelX,imageSobelY,pointDirection);
    Mat SobelGradAmpl;

    //计算X、Y方向梯度融合幅值
    SobelAmplitude(imageSobelX,imageSobelY,SobelGradAmpl);
    Mat imageLocalMax;
    imshow("XYRange",SobelGradAmpl);

    //局部非极大值抑制
    LocalMaxValue(SobelGradAmpl,imageLocalMax,pointDirection);
    imshow("Non-Maximum_Image",imageLocalMax);
    Mat cannyImage;
    cannyImage=Mat::zeros(imageLocalMax.size(),CV_8UC1);

    //双阈值处理
    DoubleThreshold(imageLocalMax,90,160);
    imshow("DoubleThreshold_Image",imageLocalMax);

    //双阈值中间阈值滤除及连接
    DoubleThresholdLink(imageLocalMax,90,160);
    imshow("Canny Image",imageLocalMax);


     //霍夫直线检测
    vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
    HoughLines(imageLocalMax, lines, 1, CV_PI/180, 150, 0, 0 );
    //依次在图中绘制出每条线段
    for( size_t i = 0; i < lines.size(); i++ ) {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cv::line( imageLocalMax, pt1, pt2, Scalar(255,128,0), 5, LINE_AA);
        }
    imshow("千辛万苦得到的图片", imageLocalMax);
    waitKey(0);
    destroyAllWindows();

    return 0;
}

//函数定义
//灰度转换函数
void ConvertRGBToGRAY(const Mat &image,Mat &image_gray) {
    //第一个参数输入RGB图像；
    //第二个参数输出灰度图像；
    if (!image.data||image.channels()!=3)
        return ;
    image_gray=Mat::zeros(image.size(),CV_8UC1);
    uchar *pointImage=image.data;
    uchar *pointImageGray=image_gray.data;
    int stepImage=image.step;
    int stepImageGray=image_gray.step;
    //遍历实现转换操作
    for(int i=0;i<image_gray.rows;i++) {
        for(int j=0;j<image_gray.cols;j++) {
            pointImageGray[i*stepImageGray+j]=0.114*pointImage[i*stepImage+3*j]+0.587*pointImage[i*stepImage+3*j+1]+0.299*pointImage[i*stepImage+3*j+2];
        }
    }
}


//高斯卷积核生成函数
void GetGaussianKernel(double **gaus, const int size,const double sigma) {
    //第一个参数gaus是一个指向含有N个double类型数组的指针；
    //第二个参数size是高斯卷积核的尺寸大小；
    //第三个参数sigma是卷积核的标准差
    const double PI=4.0*atan(1.0); //圆周率π赋值
    int center=size/2;
    double sum=0;
    for (int i=0;i<size;++i) {
        for (int j=0;j<size;++j) {
            gaus[i][j]=(1/(2*PI*sigma*sigma))*exp(-((i-center)*(i-center)+(j-center)*(j-center))/(2*sigma*sigma));
            sum+=gaus[i][j];
        }
    }
    for (int i=0;i<size;++i) {
        for (int j=0;j<size;++j) {
            gaus[i][j]/=sum;
            cout<<gaus[i][j]<<"  ";
        }
        cout<<endl<<endl;
    }
}


//高斯滤波
void GaussianFilter(const Mat imageSource,Mat &imageGaussian,double **gaus,int size) {
    //imageSource是原始图像；
    //imageGaussiangao是输出图像；
    //gaus是一个指向含有N个double类型数组的指针；
    //size是滤波核的尺寸
    imageGaussian=Mat::zeros(imageSource.size(),CV_8UC1);
    if (!imageSource.data||imageSource.channels()!=1)
        return;
    double gausArray[100];
    for (int i=0;i<size*size;i++)
        gausArray[i]=0;  //赋初值，空间分配
    int array=0;
    for (int i=0;i<size;++i) {    //system("pause");
        for (int j=0;j<size;++j) {
            gausArray[array]=gaus[i][j];//二维数组到一维 方便计算
            ++array;
        }
    }
    //滤波
    for(int i=0;i<imageSource.rows;++i) {
        for(int j=0;j<imageSource.cols;++j) {
            int k=0;
            for(int l=-size/2;l<=size/2;++l) {
                for(int g=-size/2;g<=size/2;++g) {
                    //以下处理针对滤波后图像边界处理，为超出边界的值赋值为边界值
                    int row=i+l;
                    int col=j+g;
                    row=row<0?0:row;
                    row=row>=imageSource.rows?imageSource.rows-1:row;
                    col=col<0?0:col;
                    col=col>=imageSource.cols?imageSource.cols-1:col;
                    //卷积和
                    imageGaussian.at<uchar>(i,j)+=gausArray[k]*imageSource.at<uchar>(row,col);
                    ++k;
                    int k=0;
                }
            }
        }
    }
}

//计算X、Y方向梯度和梯度方向角
void SobelGradDirction(const Mat imageSource,Mat &imageSobelX,Mat &imageSobelY,double *&pointDrection) {
    //imageSourc原始灰度图像；
    //imageSobelX是X方向梯度图像；
    //imageSobelY是Y方向梯度图像；
    //pointDrection是梯度方向角数组指针
    pointDrection=new double[(imageSource.rows-1)*(imageSource.cols-1)];
    for (int i=0;i<(imageSource.rows-1)*(imageSource.cols-1);++i)
        pointDrection[i]=0;
    imageSobelX=Mat::zeros(imageSource.size(),CV_32SC1);
    imageSobelY=Mat::zeros(imageSource.size(),CV_32SC1);
    uchar *P=imageSource.data;
    uchar *PX=imageSobelX.data;
    uchar *PY=imageSobelY.data;

    int step=imageSource.step;
    int stepXY=imageSobelX.step;
    int k=0;
    int m=0;
    int n=0;
    for(int i=1;i<(imageSource.rows-1);++i) {
        for(int j=1;j<(imageSource.cols-1);++j) {
            //通过指针遍历图像上每一个像素
            double gradY=P[(i-1)*step+j+1]+P[i*step+j+1]*2+P[(i+1)*step+j+1]-P[(i-1)*step+j-1]-P[i*step+j-1]*2-P[(i+1)*step+j-1];
            PY[i*stepXY+j*(stepXY/step)]=abs(gradY);
            double gradX=P[(i+1)*step+j-1]+P[(i+1)*step+j]*2+P[(i+1)*step+j+1]-P[(i-1)*step+j-1]-P[(i-1)*step+j]*2-P[(i-1)*step+j+1];
            PX[i*stepXY+j*(stepXY/step)]=abs(gradX);
            if(gradX==0)
                gradX=0.00000000000000001;  //防止除数为0异常
            pointDrection[k]=atan(gradY/gradX)*57.3;//弧度转换为度
            pointDrection[k]+=90;
            ++k;
        }
    }
    convertScaleAbs(imageSobelX,imageSobelX);
    convertScaleAbs(imageSobelY,imageSobelY);
}

//计算Sobel的X和Y方向梯度幅值
void SobelAmplitude(const Mat imageGradX,const Mat imageGradY,Mat &SobelAmpXY) {
    //imageGradX是X方向梯度图像；
    //imageGradY是Y方向梯度图像；
    //SobelAmpXY是输出的X、Y方向梯度图像幅值
    SobelAmpXY=Mat::zeros(imageGradX.size(),CV_32FC1);
    for (int i=0;i<SobelAmpXY.rows;++i){
        for (int j=0;j<SobelAmpXY.cols;++j) {
            SobelAmpXY.at<float>(i,j)=sqrt(imageGradX.at<uchar>(i,j)*imageGradX.at<uchar>(i,j)+imageGradY.at<uchar>(i,j)*imageGradY.at<uchar>(i,j));
        }
    }
    convertScaleAbs(SobelAmpXY,SobelAmpXY);
}

//局部极大值抑制
void LocalMaxValue(const Mat imageInput,Mat &imageOutput,double *pointDrection) {
    //imageInput输入的Sobel梯度图像；
    //imageOutPut是输出的局部极大值抑制图像；
    //pointDrection是图像上每个点的梯度方向数组指针
    imageOutput=imageInput.clone();
    int k=0;
    for(int i=1;i<imageInput.rows-1;++i) {
        for(int j=1;j<imageInput.cols-1;++j) {
            int value00=imageInput.at<uchar>((i-1),j-1);
            int value01=imageInput.at<uchar>((i-1),j);
            int value02=imageInput.at<uchar>((i-1),j+1);
            int value10=imageInput.at<uchar>((i),j-1);
            int value11=imageInput.at<uchar>((i),j);
            int value12=imageInput.at<uchar>((i),j+1);
            int value20=imageInput.at<uchar>((i+1),j-1);
            int value21=imageInput.at<uchar>((i+1),j);
            int value22=imageInput.at<uchar>((i+1),j+1);

            if(pointDrection[k]>0&&pointDrection[k]<=45) {
                if(value11<=(value12+(value02-value12)*tan(pointDrection[i*imageOutput.rows+j]))||(value11<=(value10+(value20-value10)*tan(pointDrection[i*imageOutput.rows+j])))) {
                    imageOutput.at<uchar>(i,j)=0;
                }
            }
            if(pointDrection[k]>45&&pointDrection[k]<=90) {
                if(value11<=(value01+(value02-value01)/tan(pointDrection[i*imageOutput.cols+j]))||value11<=(value21+(value20-value21)/tan(pointDrection[i*imageOutput.cols+j]))) {
                    imageOutput.at<uchar>(i,j)=0;
                }
            }
            if(pointDrection[k]>90&&pointDrection[k]<=135) {
                if(value11<=(value01+(value00-value01)/tan(180-pointDrection[i*imageOutput.cols+j]))||value11<=(value21+(value22-value21)/tan(180-pointDrection[i*imageOutput.cols+j]))) {
                    imageOutput.at<uchar>(i,j)=0;
                }
            }
            if(pointDrection[k]>135&&pointDrection[k]<=180) {
                if(value11<=(value10+(value00-value10)*tan(180-pointDrection[i*imageOutput.cols+j]))||value11<=(value12+(value22-value11)*tan(180-pointDrection[i*imageOutput.cols+j]))) {
                    imageOutput.at<uchar>(i,j)=0;
                }
            }
            ++k;
        }
    }
}

//双阈值处理
void DoubleThreshold(Mat &imageIput,double lowThreshold,double highThreshold) {
    //第一个参数imageInput输入和输出的的Sobel梯度幅值图像；
    //第二个参数lowThreshold是低阈值
    //第三个参数highThreshold是高阈值
    for(int i=0;i<imageIput.rows;++i) {
        for(int j=0;j<imageIput.cols;++j) {
            if(imageIput.at<uchar>(i,j)>highThreshold)
                imageIput.at<uchar>(i,j)=255;
            if(imageIput.at<uchar>(i,j)<lowThreshold)
                imageIput.at<uchar>(i,j)=0;
        }
    }
}

//双阈值中间像素连接处理
void DoubleThresholdLink(Mat &imageInput,double lowThreshold,double highThreshold) {
    //imageInput输入和输出的的Sobel梯度幅值图像；
    //lowThreshold是低阈值
    //highThreshold是高阈值
    for(int i=1;i<imageInput.rows-1;++i) {
        for(int j=1;j<imageInput.cols-1;++j) {
            if(imageInput.at<uchar>(i,j)>lowThreshold&&imageInput.at<uchar>(i,j)<255) {
                if(imageInput.at<uchar>(i-1,j-1)==255||imageInput.at<uchar>(i-1,j)==255||imageInput.at<uchar>(i-1,j+1)==255||
                    imageInput.at<uchar>(i,j-1)==255||imageInput.at<uchar>(i,j)==255||imageInput.at<uchar>(i,j+1)==255||
                    imageInput.at<uchar>(i+1,j-1)==255||imageInput.at<uchar>(i+1,j)==255||imageInput.at<uchar>(i+1,j+1)==255) {
                    imageInput.at<uchar>(i,j)=255;
                    DoubleThresholdLink(imageInput,lowThreshold,highThreshold); //递归调用
                }
                else {
                    imageInput.at<uchar>(i,j)=0;
                }
            }
        }
    }
}


//--------------分割线------------后面这两个函数不知道对不，反正跑不了------------------------------------
//霍夫变换函数
vector<struct line> houghLine(Mat &img, int threshold)
{
    vector<struct line> lines;
    int diagonal = floor(sqrt(img.rows*img.rows + img.cols*img.cols));
    vector< vector<int> >p(360 ,vector<int>(diagonal));

    for( int j = 0; j < img.rows ; j++ ) {
        for( int i = 0; i < img.cols; i++ ) {
            if( img.at<unsigned char>(j,i) > 0)
            {
                for(int theta = 0;theta < 360;theta++)
                {
                    int r = floor(i*cos(theta/RADIAN) + j*sin(theta/RADIAN));
                    if(r < 0)
                        continue;
                    p[theta][r]++;
                }
            }
        }
    }

    //get local maximum
    for( int theta = 0;theta < 360;theta++) {
        for( int r = 0;r < diagonal;r++) {
            int thetaLeft = max(0,theta-1);
            int thetaRight = min(359,theta+1);
            int rLeft = max(0,r-1);
            int rRight = min(diagonal-1,r+1);
            int tmp = p[theta][r];
            if( tmp > threshold
                && tmp > p[thetaLeft][rLeft] && tmp > p[thetaLeft][r] && tmp > p[thetaLeft][rRight]
                && tmp > p[theta][rLeft] && tmp > p[theta][rRight]
                && tmp > p[thetaRight][rLeft] && tmp > p[thetaRight][r] && tmp > p[thetaRight][rRight])
            {
                struct line newline;
                newline.theta = theta;
                newline.r = r;
                lines.push_back(newline);
            }
        }
    }

    return lines;
}


//这个是画线的
void drawLine(Mat &img,vector<Vec2f>lines,Scalar scalar,int n) {
    //要标记的图像
    //检测的直线数据
    //绘制直线的颜色
    //绘制直线的线宽
    double rows=img.rows;
    double cols=img.cols;
    Point pt1,pt2;
    for(size_t i=0;i<lines.size();++i){
        float rho=lines[i][0];//直线距离坐标原点的距离
        float theta=lines[i][1];//直线过坐标原点垂线与x轴夹角
        double a=cos(theta);//夹角的余弦值
        double b=sin(theta);//夹角的正弦值
        double x0=a*rho,y0=b*rho;//直线与坐标原点垂线的交点
        double length=max(rows,cols);//图像高宽的最大值

        //计算直线上的一点
        pt1.x=cvRound(x0+length*(-b));
        pt1.y=cvRound(y0+length*(a));
        //计算直线上另一点
        pt2.x=cvRound(x0-length*(-b));
        pt2.y=cvRound(y0-length*(a));
        //两点绘制一条直线
        cv::line(img,pt1,pt2,scalar,n);
    }
}










