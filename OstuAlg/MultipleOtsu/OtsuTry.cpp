#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


vector<double> getHist(Mat img){
    vector<double> hist(256,0.0f);
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            hist.at(img.at<uchar>(i,j))++;
        }
    }
    for(int k=0;k<hist.size();k++){
        hist.at(k) /= img.cols*img.rows;
    }
    return hist;
}

vector<int> Otsu(vector<double> hist){
    double CumMean = 0.0f;
    for(int i=0;i<hist.size();i++){
        CumMean += hist.at(i)*i;
    }
    vector<double> Prob(3,0.0f);
    vector<double> CurrMean(3,0.0f);
    vector<int> th(2,0);
    double MaxVal = 0.0f;
    double CurrVal = 0.0f;
    for(int i=0;i<255-2;i++){
        Prob[0] += hist.at(i);
        CurrMean[0] += hist.at(i)*i;
        for(int j=i+1;j<255-1;j++){
            Prob[1] += hist.at(j);
            CurrMean[1] += hist.at(j)*j;
            for(int k=j+1;k<255;k++){
                Prob[2] += hist.at(k);
                CurrMean[2] += hist.at(k)*k;
                CurrVal = 0.0f;
                for(int w=0;w<3;w++){
                    CurrVal += Prob[w]* pow((CurrMean[w] - CumMean),2);
                }
                if(CurrVal > MaxVal){
                    MaxVal = CurrVal;
                    th[0] = i;
                    th[1] = j;
                }
            }
            Prob[2] = CurrMean[2] = 0.0f;
        }
        Prob[1] = CurrMean[1] = 0.0f;
    }
    cout<<th[0]<<endl;
    cout<<th[1]<<endl;
    return th;
}


Mat ApplyMultipleTh(Mat img,vector<int> OtsuTh){
    Mat output = Mat::zeros(img.size(),img.type());
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            if(img.at<uchar>(i,j) > OtsuTh[0] && img.at<uchar>(i,j)< OtsuTh[1])
                output.at<uchar>(i,j) = 127;
            else if(img.at<uchar>(i,j)> OtsuTh[1])
                output.at<uchar>(i,j) = 255;
        }
    }
    return output;
}


int main(int argc,char** argv){
    Mat img = imread(argv[1],IMREAD_GRAYSCALE);
    if(img.empty())
        return -1;
    
    GaussianBlur(img,img,Size(5,5),0,0);

    vector<double> hist = getHist(img);
    Mat output = ApplyMultipleTh(img,Otsu(hist));
    imshow("output",output);
    waitKey();
    return 0;
}