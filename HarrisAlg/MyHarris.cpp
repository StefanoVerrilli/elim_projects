#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc,char** argv){
    Mat img = imread(argv[1],IMREAD_GRAYSCALE);
    if(img.empty()){
        cout<<"Usage:"<<argv[0]<<"image name"<<endl;
        return -1;
    }

    Mat Dx,Dy;
    Sobel(img,Dx,CV_32FC1,1,0,3);
    Sobel(img,Dy,CV_32FC1,0,1,3);

    Mat Dx2,Dy2,Dxy;
    pow(Dx,2,Dx2);
    pow(Dy,2,Dy2);
    multiply(Dx,Dy,Dxy);

    GaussianBlur(Dx2,Dx2,Size(7,7),0,0);
    GaussianBlur(Dy2,Dy2,Size(7,7),0,0);
    GaussianBlur(Dxy,Dxy,Size(7,7),0,0);

    Mat trace,R,Dx2Dy2,Dxy2;
    multiply(Dx2,Dy2,Dx2Dy2);
    multiply(Dxy,Dxy,Dxy2);
    pow((Dx2 + Dy2),2,trace);
    R = (Dx2Dy2 - Dxy2) - 0.04*trace;

    normalize(R,R,0,255,NORM_MINMAX,CV_32FC1);

    Mat output = img.clone();

    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            if((int)R.at<float>(i,j) > 100)
                circle(output,Point(j,i),3,Scalar(255),2,8,0);
        }
    }
    
    imshow("output",output);
    waitKey();
    return 0;
}