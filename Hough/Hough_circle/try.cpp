#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc,char**argv){

if(argc<2){
	cout<<"Usage"<<argv[0]<<"Image"<<endl;
	exit(0);
}
Mat img = imread(argv[1],IMREAD_GRAYSCALE);
if(img.empty()){
	cout<<"Error image not found"<<endl;
	exit(0);
}

Mat GaussImage;
GaussianBlur(img,GaussImage,Size(7,7),0,0);

Mat CannyImage;
Canny(GaussImage,CannyImage,35,85);

double theta,rho;
int disp = hypot(CannyImage.rows,CannyImage.cols);
Mat votes = Mat::zeros(disp*2,180,CV_8U);

for(int x =0;x<CannyImage.rows;x++){
	for(int y = 0;y<CannyImage.cols;y++){
		if(CannyImage.at<uchar>(x,y)==255){
			for(theta =0;theta<180;theta++){
				rho = disp + x*sin((theta-90)*CV_PI/180) + y*cos((theta-90)*CV_PI/180);
				votes.at<uchar>(rho,theta)++;
			}
		}
	}
}

for(int r = 0;r<votes.rows;r++){
	for(int t = 0;t<votes.cols;t++){
		if(votes.at<uchar>(r,t)>=100){
			theta = (disp-t)*CV_PI/180;
			int x = disp + r*cos(theta);
			int y = disp + r*sin(theta);
			double sin_t = sin(theta);
			double cos_t = cos(theta);
			Point pt1(cvRound(x + dist*(-sin_t)),cvRound(y + dist*(cos_t)));
			Point pt2(cvRound(x - dist*(-sin_t)),cvRound(y - dist*(cos_t)));
			line(img,pt1,pt2,Scalar(0),8,0);
		}

	}
}

}