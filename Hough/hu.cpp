#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main(int argc,char**argv){


if(argc<2){
	cout<<"Usage:"<<argv[1]<<" image"<<endl;
	exit(0);
}

Mat image = imread(argv[2],IMREAD_GRAYSCALE);
if(image.empty()){
	cout<<"Error image not found"<<endl;
	exit(0);
}

Mat GaussImage;
GaussianBlur(image,GaussImage,Size(5,5),0,0);


Mat CannyImage;
Canny(GaussImage,CannyImage,35,80);

int theta,rho;
int dist = hypot(CannyImage.rows,CannyImage.cols);
Mat votes = Mat::zeros(dist*2,180,CV_8U);
for(int x = 0;x<CannyImage.rows;x++){
	for(int y=0;y<CannyImage.cols;y++){
		if(CannyImage.at<uchar>(x,y)==255){
			for(theta=0;theta<180;theta++){
				rho = dist + x*cos((theta-90)*CV_PI/180) + y*sin((theta-90)*CV_PI/180);
				votes.at<uchar>(rho,theta)++;
			}

		}
	}
}

for(int r=0;r<votes.rows;r++){
	for(int t=0;t<votes.cols;t++){
		if(votes.at<uchar>(r,t)>=200){
			theta = (t-90)*CV_PI/180;
			int x = (r-dist)*cos(theta);
			int y = (r-dist)*sin(theta);
			double sin_t = sin(theta);
			double cos_t = cos(theta);
			Point pt1(cvRound(x + dist*(-sin_t)),cvRound(y + disp*(cos_t)));
			Point pt2(cvRound(x - dist*(-sin_t)),cvRound(y - disp*(cos_t)));
			line(image,pt1,pt2,Scalar(0),2,0);
		}
	}
}
//rho = x*cos(theta)+y*sin(theta)
//rho/cos(theta)= x +y*sin(theta)/cos(theta);
//x = r/cos(theta) - y*sin(theta)/cos(theta);
//x = (r - y)*cos(theta);

//x*cos(theta)= y*sin(theta) - rho	

//rho = x*cos(theta)+ y*sin(theta);
//rho/sin(theta) = x*cos(theta)/sin(theta) +y
//rho/sin(theta) - x*cos(theta)/sin(theta) = y
//y = (rho - x)*sin(theta);
namedWindow("output",WINDOW_AUTOSIZE);
imshow("output",image);

waitKey(0);


}