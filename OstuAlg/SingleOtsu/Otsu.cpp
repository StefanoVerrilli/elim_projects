#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;




int Otsu(vector<double> hist){
    double mediaCum = 0.0f;
    int th = 0;
    for(int i=0;i<hist.size();i++){
        mediaCum+=hist.at(i)*i;
    }
    double prob = 0.0f;
    double CurrMediaCum = 0.0f;
    double currVal = 0.0f;
    double MaxVal = 0.0f;
    for(int i=0;i<hist.size();i++){
        prob+=hist.at(i);
        CurrMediaCum += i*hist.at(i);
        currVal = pow((mediaCum*prob - CurrMediaCum),2)/(prob*(1-prob));
        if(currVal > MaxVal){
            MaxVal = currVal;
            th = i;
        }
    }
    return th;
}

vector<double> getHist(Mat img){
    vector<double> hist(256,0.0f);
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            hist.at(img.at<uchar>(i,j))++;
        }
    }
    for(int i=0;i<256;i++){
        hist.at(i)/= img.cols*img.rows;
    }
    return hist;
}

int main(int argc,char** argv){
    Mat img = imread(argv[1],IMREAD_GRAYSCALE);
    if(img.empty()){
        cout<<"error"<<endl;
        return -1;
    }

    Mat BlurredImage;
    GaussianBlur(img,BlurredImage,Size(5,5),0,0);
    vector<double> hist = getHist(BlurredImage);

    Mat out;
    threshold(BlurredImage,out,Otsu(hist),255,THRESH_BINARY);

    imshow("output",out);
    waitKey();
    return 0;
}