#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int Otsu(vector<double> hist){
    double MediaCum=0.0f;
    int th=0;
    for(int i=0;i<hist.size();i++){
        MediaCum += hist.at(i)*i;
    }
    double Prob = 0.0f;
    double CurrMean =0.0f;
    double MaxVal = 0.0f;
    double CurrVal = 0.0f;
    for(int i=0;i<256;i++){
        Prob += hist.at(i);
        CurrMean += hist.at(i)*i;
        CurrVal = pow(MediaCum*Prob - CurrMean,2)/Prob*(1-Prob);
        if(CurrVal > MaxVal){
            th = i;
            MaxVal = CurrVal;
        }
    }
    return th;
}

vector<double> getHist(Mat img){
    vector<double> hist(256,0.0f);
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            hist.at(img.at<uchar>(j,i))++;
        }
    }
    for(int i=0;i<hist.size();i++){
        hist.at(i) /= img.rows*img.cols;
    }
}

int main(int argc,char** argv){
    Mat img = imread(argv[1],IMREAD_GRAYSCALE);

    if(img.empty()){
        return -1;
    }

    GaussianBlur(img,img,Size(5,5),0,0);

    vector<double> hist = getHist(img);
    threshold(img,img,Otsu(hist),255,THRESH_BINARY);
}