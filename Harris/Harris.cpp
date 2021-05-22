#include <iostream>
#include <opencv2/opencv.hpp>
#include "Utility.hpp"

using namespace cv;
using namespace std;


int	 main(int argc,char**argv){


if(argc<2){
	cout<<"Usage:"<<argv[1]<<" image"<<endl;
	exit(0);
}

Mat image = imread(argv[1],IMREAD_GRAYSCALE);
if(image.empty()){
	cout<<"Error image not found"<<endl;
	exit(0);
}

	double k=0.04;
	Mat Dx,Dy;
	Sobel(image,Dx,CV_32F,1,0,3,BORDER_DEFAULT);
	Sobel(image,Dy,CV_32F,0,1,3,BORDER_DEFAULT);

	Mat Dx2,Dy2,DxDy;
	pow(Dx,2,Dx2);
	pow(Dy,2,Dy2);
	multiply(Dx,Dy,DxDy);

	Mat Dx2g,Dy2g,DxDyg;
	GaussianBlur(Dx2,Dx2g,Size(7,7),2.0,0.0,BORDER_DEFAULT);
	GaussianBlur(Dy2,Dy2g,Size(7,7),0.0,2.0,BORDER_DEFAULT);
	GaussianBlur(DxDy,DxDyg,Size(7,7),2.0,2.0,BORDER_DEFAULT);

	Mat x2y2,xy,trace,R;
	multiply(Dx2g,Dy2g,x2y2);
	multiply(DxDyg,DxDyg,xy);
	pow((Dx2g+Dy2g),2.0,trace);
	R = ((x2y2-xy) - k* trace);

	normalize(R,R,0,255,NORM_MINMAX,CV_32FC1);

	Mat out= image.clone();
	int thr = 100;
	for(int i=0;i<image.rows;i++){
		for(int j=0;j<image.cols;j++){
			if((int)R.at<float>(i,j) > thr)
				circle(out,Point(j,i),5,Scalar(255),2,8,0);
		}
	}
		namedWindow("Harris result",WINDOW_AUTOSIZE);
		imshow("Harris result",out);
		waitKey(0);

}