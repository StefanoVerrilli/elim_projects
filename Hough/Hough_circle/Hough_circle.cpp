#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

int main(int argc,char**argv){

if(argc<2){
	cout<<"Usage:"<<argv[0]<<" Image"<<endl;
	exit(0);
}

Mat img= imread(argv[1],IMREAD_GRAYSCALE);
if(img.empty()){
	cout<<"Error image not found"<<endl;
	exit(0);
}


Mat GaussImage;
GaussianBlur(img,GaussImage,Size(7,7),0,0);

Mat Canny_image;
Canny(GaussImage,Canny_image,150,230,3);

cout<<Canny_image.rows<<endl;
cout<<Canny_image.cols<<endl;

int theta;
int r_min=10;
int r_max=100;
int size[]={img.rows,img.cols,r_max-r_min+1};
Mat votes = Mat::zeros(3,size,CV_8U);

cout<<votes.size<<endl;


for(int x = 0;x<Canny_image.rows;x++){
	for(int y =0;y<Canny_image.cols;y++){
		if(Canny_image.at<uchar>(x,y)==255){
			for(int r=r_min;r<r_max;r++){
				for(theta=0;theta<360;theta++){
					int a = y + r*cos(theta*CV_PI/180);
					int b = x + r*sin(theta*CV_PI/180);
					if(a>=0 && a<Canny_image.cols && b>=0 && b<Canny_image.rows)
					votes.at<uchar>(b,a,r-r_min)++; 
				}
			}
		}
	}
}

for(int r=r_min;r<r_max;r++){
	for(int b=0;b<Canny_image.rows;b++){
		for(int a=0;a<Canny_image.cols;a++){
			if(votes.at<uchar>(b,a,r-r_min)> 170){
				circle(img,Point(a,b),3,Scalar(255),0,8,0);
				circle(img,Point(a,b),r,Scalar(255),0,8,0);
			}

		}
	}
}

namedWindow("Canny edge detection",WINDOW_AUTOSIZE);
imshow("Canny edge detection",Canny_image);

namedWindow("Circle detection",WINDOW_AUTOSIZE);
imshow("Circle detection",img);

waitKey(0);
return 0;


}