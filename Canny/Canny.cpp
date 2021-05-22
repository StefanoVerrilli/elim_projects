#include <iostream>
#include <opencv2/opencv.hpp>
#include "../Utility.hpp"

using namespace cv;
using namespace std;


void thresholding(Mat img,Mat &out,int lth,int hth){
	for(int i = 1; i<img.rows;i++){
		for(int j=1;j<img.cols;j++){
			if(img.at<uchar>(i,j)> hth){
				for(int surr_iter=-1;surr_iter<=1;surr_iter++){
					for(int surr_iter2=-1;surr_iter2<=1;surr_iter2++){
						if(img.at<uchar>(i+surr_iter,j+surr_iter2)> lth && img.at<uchar>(i+surr_iter,j+surr_iter2)< hth){
							out.at<uchar>(i+surr_iter,j+surr_iter2)=255;
						}
					}
				}
			}
			else if(img.at<uchar>(i,j) < lth){
					out.at<uchar>(i,j) = 0;}
		}
	}
}

void NoMaxSuppress(Mat mag,Mat orientation,Mat &out){

	for(int i=1;i<mag.rows-1;i++){
		for(int j=1;j<mag.cols-1;j++){
			float angle = orientation.at<float>(i,j);
			angle=angle>180?angle-360:angle;
			if((angle<22.5 && angle>-22.5) || (angle<157.5 && angle>-157.5)){
				if((mag.at<float>(i,j+1)<mag.at<float>(i,j)) && mag.at<float>(i,j-1)<mag.at<float>(i,j)){
					out.at<float>(i,j)=mag.at<float>(i,j);
				}
			}
			else if((angle>22.5 && angle<67.5) || (angle<-112.5 && angle>-157.5)){
				if((mag.at<float>(i+1,j-1)<mag.at<float>(i,j)) && mag.at<float>(i-1,j-1)<mag.at<float>(i,j)){
					out.at<float>(i,j)=mag.at<float>(i,j);
				}
			}
			else if((angle>67.5 && angle<112.5) || (angle<-67.5 && angle>-112.5)){
				if((mag.at<float>(i,j-1)<mag.at<float>(i,j)) && mag.at<float>(i,j+1)<mag.at<float>(i,j)){
					out.at<float>(i,j)=mag.at<float>(i,j);
				}
			}
			else if((angle>112.5 && angle<157.5) || (angle<-22.5 && angle>-67.5)){
				if((mag.at<float>(i-1,j-1)<mag.at<float>(i,j)) && mag.at<float>(i+1,j+1)<mag.at<float>(i,j)){	
					out.at<float>(i,j)=mag.at<float>(i,j);
				}
			}
		}
	}
}

int main(int argc,char**argv){

if(argc<2){
	cout<<"Usage:"<<argv[1]<<" Image"<<endl;
	exit(0);

}
Mat image = imread(argv[1],IMREAD_GRAYSCALE);
if(image.empty()){
	cout<<"Error image not found"<<endl;
	exit(0);
}

Mat ImageGauss;
GaussianBlur(image,ImageGauss,Size(5,5),2.0,2.0,BORDER_DEFAULT);

Mat Dx,Dy,Mag;
Sobel(ImageGauss,Dx,CV_32F,1,0,3,BORDER_DEFAULT);
Sobel(ImageGauss,Dy,CV_32F,0,1,3,BORDER_DEFAULT);

Mat Dx2,Dy2;
pow(Dx,2,Dx2);
pow(Dy,2,Dy2);
sqrt(Dx2+Dy2,Mag);
normalize(Mag,Mag,0,255,NORM_MINMAX,CV_8U);

Mat orentation;
phase(Dx,Dy,orentation,true);

Mat nms = Mat::zeros(Mag.rows,Mag.cols,CV_8U);
NoMaxSuppress(Mag,orentation,nms);

Mat out = Mat::zeros(Mag.rows,Mag.cols,CV_8U);
thresholding(nms,out,35,85);

namedWindow("Image Canny result",WINDOW_AUTOSIZE);
imshow("Image Canny result",out);

waitKey(0);

}