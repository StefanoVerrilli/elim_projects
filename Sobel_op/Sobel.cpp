#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void Sobel(Mat img,Mat &output,int weightX,int weighty);
Mat Convolution(Mat img,Mat filter);
void Mine_magnitude(Mat img_g_x,Mat img_g_y,Mat &magnitude);
void abs_Magnitude(Mat img_g_x,Mat img_g_y,Mat &output);

int main(int argc, char**argv){

  if(argc<2){
    cout<<"usage: "<<argv[0]<<" image_name"<<endl;
    exit(0);
  }

  Mat image =imread(argv[1],IMREAD_GRAYSCALE);
  if(image.empty()){
    cout<<"Error, image not found"<<endl;
    return -1;
  }
  Mat SobelX,SobelY,SobelXY;
  Mat SobelX_right,SobelY_right,SobelXY_right;
  Mat Mag_image,abs_magnitude;

  //Applying sobel filter with partial derivative g_x and g_y
  Sobel(image,SobelX,1,0);
  Sobel(image,SobelX_right,image.type(),1,0);

  Sobel(image,SobelY,0,1);
  Sobel(image,SobelY_right,image.type(),0,1);

  Sobel(image,SobelXY,1,1);
  Sobel(image,SobelXY_right,image.type(),1,1);

  Mine_magnitude(SobelX,SobelY,Mag_image);
  abs_Magnitude(SobelX,SobelY,abs_magnitude);

  /*convertScaleAbs(SobelX, SobelX);
  convertScaleAbs(SobelY, SobelY);
  convertScaleAbs(SobelXY, SobelXY);*/

  normalize(SobelX,SobelX,0,255,NORM_MINMAX,CV_8UC1);
  normalize(SobelY,SobelY,0,255,NORM_MINMAX,CV_8UC1);
  normalize(SobelXY,SobelXY,0,255,NORM_MINMAX,CV_8UC1);

  namedWindow("SobelX result",WINDOW_AUTOSIZE);
  imshow("SobelX result",SobelX);

  namedWindow("SobelY result",WINDOW_AUTOSIZE);
  imshow("SobelY result",SobelY);

  namedWindow("SobelXY result",WINDOW_AUTOSIZE);
  imshow("SobelXY result",SobelXY);

  namedWindow("Magnitude_abs",WINDOW_AUTOSIZE);
  imshow("Magnitude_abs",abs_magnitude);

  namedWindow("Magnitude",WINDOW_AUTOSIZE);
  imshow("Magnitude",Mag_image);

  Mat sharpening = image + Mag_image;

  namedWindow("Original image after sharpening",WINDOW_AUTOSIZE);
  imshow("Original image after sharpening",sharpening);

  waitKey(0);
  return 0;

}


void Sobel(Mat img,Mat &output,int weightX,int weighty){
  int kernel_dims=3;

  float data_g_x[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};
  Mat g_x= Mat(kernel_dims,kernel_dims,CV_32F,data_g_x)*weightX;
  cout<<"g_x filter:\n"<<g_x<<endl;

  float data_g_y[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
  Mat g_y = Mat(kernel_dims,kernel_dims,CV_32F,data_g_y)*weighty;
  cout<<"g_y filter:\n"<<g_y<<endl;

  output = Convolution(img,g_x) + Convolution(img,g_y);

}

Mat Convolution(Mat img,Mat filter){
  int pad=filter.rows/2;
  Mat padded_img;
  Mat output(img.rows,img.cols,filter.type());
  copyMakeBorder(img,padded_img,pad,pad,pad,pad,BORDER_CONSTANT,Scalar(0));
  padded_img.convertTo(padded_img,filter.type());
  for(int image_iter_rows=0;image_iter_rows<img.rows;image_iter_rows++){
    for(int image_iter_cols=0;image_iter_cols<img.cols;image_iter_cols++){
      float filtertotal=0.F;
      Rect r(Point(image_iter_cols,image_iter_rows),Point(image_iter_cols+filter.cols,image_iter_rows+filter.rows));
      Mat block = padded_img(r);
      Mat product = block.mul(filter);
      for(int block_rows=0;block_rows<filter.rows;block_rows++){
        for(int block_cols=0;block_cols<filter.cols;block_cols++){
          filtertotal+=product.at<float>(block_rows,block_cols);
        }}
        output.at<float>(image_iter_rows,image_iter_cols)=filtertotal;
      }
    }
    return output;
}

void Mine_magnitude(Mat img_g_x,Mat img_g_y,Mat &magnitude){
  Mat pow1,pow2;
  pow(img_g_x,2,pow1);
  pow(img_g_y,2,pow2);
  sqrt((pow1+pow2),magnitude);
  normalize(magnitude,magnitude,0,255,NORM_MINMAX,CV_8UC1);
}

void abs_Magnitude(Mat img_g_x,Mat img_g_y,Mat &output){
  Mat magnitude= abs(img_g_x)+ abs(img_g_y);
  normalize(magnitude,output,0,255,NORM_MINMAX,CV_8UC1);
}
