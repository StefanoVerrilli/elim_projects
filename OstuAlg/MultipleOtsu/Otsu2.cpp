#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



vector<int> OtsuMultipleTh(vector<double> hist){
    double MeanCum = 0.0f;
    vector<int> th(2,0);
    for(int i=0;i<256;i++){
        MeanCum += hist.at(i)*i;
    }
    vector<double> Prob(3,0.0f);
    vector<double> MeanCurr(3,0.0f);
    double CurrVal =0.0f;
    double MaxVal = 0.0f;
    for(int i=0;i<255-2;i++){
        Prob[0] += hist[i];
        MeanCurr[0] += hist[i]*i;
        for(int j=i+1;j<255-1;j++){
            Prob[1] +=hist[j];
            MeanCurr[1] += hist[j]*j;
            for(int k=j+1;k<255;k++){
                Prob[2] += hist[k];
                MeanCurr[2] += hist[k]*k;
                CurrVal = 0.0f;
                for(int w=0;w<3;w++){
                    CurrVal += Prob[w] * pow((MeanCurr[w] - MeanCum),2);
                }
                if(CurrVal > MaxVal){
                    MaxVal = CurrVal;
                    th[0] = i;
                    th[1] = j;
                }
            }
            Prob[2] = MeanCurr[2] = 0.0f;
        }
        Prob[1] = MeanCurr[1] = 0.0f;
    }
    cout<<th[0]<<endl;
    cout<<th[1]<<endl;
    return th;
}


void ApplyOtsu(Mat src,Mat &output,vector<int> th){
    Mat out = Mat::zeros(src.size(),src.type());
    for(int i=0;i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            if(src.at<uchar>(i,j) < th[0]){
                out.at<uchar>(i,j) = 0;
            }else if(src.at<uchar>(i,j) > th[1]){
                out.at<uchar>(i,j) = 255;
            }else if(src.at<uchar>(i,j) > th[0] && src.at<uchar>(i,j) < th[1]){
                out.at<uchar>(i,j) = 127;
            }
        }
    }
    output = out;
}

vector<double> getHist(Mat src){
    vector<double> hist(256,0.0f);
    for(int i=0;i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            hist.at(src.at<uchar>(i,j))++;
        }
    }
    for(int i=0;i<hist.size();i++){
        hist.at(i) /= src.cols*src.rows;
    }
    return hist;
}

int main(int argc,char** argv){
    Mat img = imread(argv[1],IMREAD_GRAYSCALE);
    if(img.empty()){
        return -1;
    }

    Mat BlurredImage;
    GaussianBlur(img,BlurredImage,Size(5,5),0,0);

    vector<double> hist = getHist(BlurredImage);
    Mat output;
    ApplyOtsu(BlurredImage,output,OtsuMultipleTh(hist));
    imshow("output",output);
    waitKey();
    return 0;

}