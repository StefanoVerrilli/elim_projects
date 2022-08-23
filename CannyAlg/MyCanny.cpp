#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


void Thresholding(Mat src,Mat &result,int min,int max){
    for(int i=1;i<src.rows-1;i++){
        for(int j=1;j<src.cols-1;j++){
            if(src.at<uchar>(i,j) > max){
                result.at<uchar>(i,j) = 255;
                for(int u=-1;u<=1;u++){
                    for(int v=-1;v<=1;v++){
                        if(src.at<uchar>(i+u,j+v) > min && src.at<uchar>(i+u,j+v) < max){
                            result.at<uchar>(i+u,j+v) = 255;
                        }
                    }
                }
            }else if(src.at<uchar>(i,j) < min){
                result.at<uchar>(i,j) = 0;
            }
        }
    }
}


void NoMaxSuppression(Mat Mag,Mat PhaseImage,Mat & result){
    for(int i=1;i<Mag.rows-1;i++){
        for(int j=1;j<Mag.cols-1;j++){
            float angle = PhaseImage.at<float>(i,j);
            angle = angle > 180 ? angle - 360 : angle;
            if((angle > -22.5 && angle < 22.5) || (angle > -157.5 && angle < 157.5)){
                if(Mag.at<uchar>(i,j) > Mag.at<uchar>(i,j+1) && Mag.at<uchar>(i,j) > Mag.at<uchar>(i,j-1))
                    result.at<uchar>(i,j) = Mag.at<uchar>(i,j);
            }
            else if((angle > 22.5 && angle < 67.5) ||(angle < -112.5 && angle > -157.5)){
                if(Mag.at<uchar>(i,j) > Mag.at<uchar>(i-1,j+1) && Mag.at<uchar>(i,j) > Mag.at<uchar>(i+1,j-1))
                    result.at<uchar>(i,j) = Mag.at<uchar>(i,j);
            }
            else if((angle > 67.5 && angle < 112.5) || (angle > -112.5 && angle < -67.5)){
                if(Mag.at<uchar>(i,j) > Mag.at<uchar>(i-1,j) && Mag.at<uchar>(i,j) > Mag.at<uchar>(i+1,j))
                    result.at<uchar>(i,j) = Mag.at<uchar>(i,j);
            }else if((angle > 112.5 && angle<157.5) || (angle < -22.5 && angle > -67.5)){
                if(Mag.at<uchar>(i,j) > Mag.at<uchar>(i+1,j+1) && Mag.at<uchar>(i,j) > Mag.at<uchar>(i-1,j-1))
                    result.at<uchar>(i,j) = Mag.at<uchar>(i,j);
            }
        }
    }
}

int main(int argc,char** argv){
    Mat img = imread(argv[1],IMREAD_GRAYSCALE);

    if(img.empty()){
        cout<<"Error"<<endl;
        return -1;
    }

    Mat BlurredImage;
    GaussianBlur(img,BlurredImage,Size(5,5),0,0);

    Mat Dx,Dy;
    Sobel(BlurredImage,Dx,CV_32FC1,1,0,3);
    Sobel(BlurredImage,Dy,CV_32FC1,0,1,3);

    Mat Dx2,Dy2,Mag;
    pow(Dx,2,Dx2);
    pow(Dy,2,Dy2);
    sqrt(Dx2+Dy2,Mag);
    normalize(Mag,Mag,0,255,NORM_MINMAX,CV_8U);

    Mat PhaseImage;
    phase(Dx,Dy,PhaseImage,true);

    Mat MaxSuppression = Mat::zeros(img.rows,img.cols,CV_8U);
    NoMaxSuppression(Mag,PhaseImage,MaxSuppression);

    Mat result = Mat::zeros(img.rows,img.cols,CV_8U);
    Thresholding(MaxSuppression,result,25,75);

    imshow("output",result);
    waitKey();
    return 0;
}