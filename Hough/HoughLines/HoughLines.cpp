#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc,char ** argv){
    Mat img = imread(argv[1]);

    if(img.empty())
        return -1;

    GaussianBlur(img,img,Size(5,5),0,0);

    Mat CannyImage;
    Canny(img,CannyImage,35,85,3);

    int th = 220;
    int dist = hypot(CannyImage.rows,CannyImage.cols);
    Mat votes = Mat::zeros(dist*2,180,CV_8U);
    double rho,theta;
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            if(CannyImage.at<uchar>(i,j) == 255){
                for(theta =0;theta<180;theta++){
                    rho = dist+j*cos((theta-90)*CV_PI/180) + i*sin((theta-90)*CV_PI/180);
                    votes.at<uchar>(rho,theta)++;
                }
            }
        }
    }

    for(int r=0;r<votes.rows;r++){
        for(int t=0;t<votes.cols;t++){
            if(votes.at<uchar>(r,t) > th){
                theta = (t-90)*CV_PI/180;
                int x = (r-dist)*cos(theta);
                int y = (r-dist)*sin(theta);
                double sin_t = sin(theta);
                double cos_t = cos(theta);
                Point pt1(cvRound(x - dist*(sin_t)),cvRound(y + dist*(cos_t)));
                Point pt2(cvRound(x + dist*(sin_t)),cvRound(y - dist*(cos_t)));
                line(img,pt1,pt2,Scalar(3),2,0);
            }
        }
    }
    imshow("img",img);
    waitKey();
    return 0;
}