#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc,char** argv){
    Mat img = imread(argv[1]);
    if(img.empty())
        return -1;

    GaussianBlur(img,img,Size(5,5),0,0);

    Mat CannyImage;
    Canny(img,CannyImage,150,230,3);

    int th = 170;
    int theta;
    int min_r = 10;
    int max_r = 100;
    int sizes[] = {CannyImage.rows,CannyImage.cols,max_r-min_r+1};
    Mat votes = Mat::zeros(3,sizes,CV_8U);
    for(int x = 0;x<img.rows;x++){
        for(int y=0;y<img.cols;y++){
            if(CannyImage.at<uchar>(x,y) == 255){
                for(int r=min_r;r<max_r;r++){
                    for(theta=0;theta<360;theta++){
                        int a = y - r*cos(theta*CV_PI/180);
                        int b = x - r*sin(theta*CV_PI/180);
                        if(a > 0 && a < img.cols && b > 0 && b < img.rows)
                            votes.at<uchar>(b,a,r-min_r)++;
                    }
                }
            }
        }
    }

    for(int r=min_r;r<max_r;r++){
        for(int b=0;b<img.rows;b++){
            for(int a = 0;a<img.cols;a++){
                if(votes.at<uchar>(b,a,r-min_r) > th){
                    circle(img,Point(a,b),r,Scalar(255),0,8,0);
                }
            }
        }
    }

    imshow("img",img);
    waitKey();
    return 0;
}