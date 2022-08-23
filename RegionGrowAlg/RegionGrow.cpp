#include <iostream>
#include <opencv2/opencv.hpp>
#include <stack>
using namespace std;
using namespace cv;

void grow(Mat &img,Mat &dest,Mat &Mask,Point seed);

const double minAreaRatio = 0.01;
const double th = 200;
const int MaxRegions = 100;
const Point shift[8]{
    Point(-1,0),
    Point(1,0),
    Point(1,1),
    Point(-1,-1),
    Point(0,1),
    Point(-1,1),
    Point(1,-1),
    Point(0,-1),
};

int main(int argc,char**argv){
    Mat img = imread(argv[1]);
    int min_area = int(minAreaRatio*img.cols*img.rows);
    int regionColor = 1;
    Mat mask = Mat::zeros(img.rows,img.cols,CV_8UC1);
    Mat dest = Mat::zeros(img.rows,img.cols,CV_8UC1);
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            if(dest.at<uchar>(Point(i,j)) == 0){
                grow(img,dest,mask,Point(i,j));
                int MaskArea = cv::sum(mask)[0];
                if(MaskArea > min_area){
                    dest += mask*(20*regionColor);
                    if(++regionColor > MaxRegions){return -1;}
                }else{
                    dest += mask*255;
                }
                mask = mask - mask;
            }
        }
    }
    imshow("final result",dest);
    waitKey();
    return 0;
}

void grow(Mat &img,Mat &dest,Mat &Mask,Point seed){
    stack<Point> Stack;
    Stack.push(seed);
    while(!Stack.empty()){
        Point front = Stack.top();
        Mask.at<uchar>(front) = 1;
        Stack.pop();
        for(int k = 0;k<8;k++){
            Point Adj = front + shift[k];
            if(Adj.x < 0 || Adj.y < 0 || Adj.x > img.cols-1 || Adj.y > img.rows-1)
                continue;
            else{
                double dist = int(pow((img.at<Vec3b>(Adj)[0] - img.at<Vec3b>(front)[0]),2) +
                pow((img.at<Vec3b>(Adj)[1] - img.at<Vec3b>(front)[1]),2) +
                pow((img.at<Vec3b>(Adj)[2] - img.at<Vec3b>(front)[2]),2));
                if(dest.at<uchar>(Adj) == 0 && Mask.at<uchar>(Adj) == 0 & dist < th){
                    Mask.at<uchar>(Adj) = 1;
                    Stack.push(Adj);
                }
            }
        }
    }
}