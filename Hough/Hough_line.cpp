#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc,char**argv){

if(argc<2){
	cout<<"Usage:"<<argv[0]<<" image"<<endl;
	exit(0);
}

Mat image = imread(argv[1],IMREAD_GRAYSCALE);
if(image.empty()){
	cout<<"Error image not found"<<endl;
	exit(0);
}
Mat image_gauss;
GaussianBlur(image,image_gauss,Size(5,5),0,0);

//Applying Canny op to find edges in the (blurred) original image to identify the lines
Mat Canny_image;
Canny(image_gauss,Canny_image,90,160,3);

namedWindow("Canny op applyed",WINDOW_AUTOSIZE);
imshow("Canny op applyed",Canny_image);

int dist = hypot(image.rows,image.cols);
Mat votes = Mat::zeros(dist*2,180,CV_8U);

double rho,theta;
for(int x=0;x<Canny_image.rows;x++){
	for(int y=0;y<Canny_image.cols;y++){
		if(Canny_image.at<uchar>(x,y)==255){
			for(theta=0;theta<180;theta++){
				rho= dist + y*cos((theta-90)*CV_PI/180) + x*sin((theta-90)*CV_PI/180);
				votes.at<uchar>(rho,theta)++;
			}

		}
	}
}

for(int r=0;r<votes.rows;r++){
	for(int t=0;t<votes.cols;t++){
		if(votes.at<uchar>(r,t)>=100){
		theta =(t-90)*CV_PI/180;
		int x = (r-dist)*cos(theta);
		int y = (r-dist)*sin(theta);
		double sin_t = sin(theta);
		double cos_t = cos(theta);
		Point pt1(cvRound(x + dist*(-sin_t)),cvRound(y + dist*cos_t));
		Point pt2(cvRound(x - dist*(-sin_t)),cvRound(y - dist*cos_t));
		line(image,pt1,pt2,Scalar(0),2,0);
		}
	}
}

namedWindow("Output",WINDOW_AUTOSIZE);
imshow("Output",image);

waitKey(0);



}