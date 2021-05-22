#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//What you will find in this header file:
void Corr_Conv(Mat img,Mat filter,Mat &return_image,int type);
void Avaraging(Mat image,Mat &output,int dim);
void Laplanceiano(Mat image,Mat &output,int isotropy,int ddepth);
void Sobel(Mat img,Mat &output,int weightX,int weighty);
void Mine_magnitude(Mat img_g_x,Mat img_g_y,Mat &magnitude);

void Corr_Conv(Mat img,Mat filter,Mat &return_image,int type){
  if(type==1)
    rotate(filter,filter,ROTATE_180);
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
    output.copyTo(return_image);
}

void Avaraging(Mat image,Mat &output,int dim){
    Mat Corr_Mean_filter=Mat::ones(dim,dim,CV_32F)/(float) (dim*dim);
    Mat imageCorrelation;
    Corr_Conv(image,Corr_Mean_filter,imageCorrelation,0);
    normalize(imageCorrelation,output,0,255,NORM_MINMAX,CV_8UC1);
}

void Laplanceiano(Mat image,Mat &output,int isotropy,int ddepth){
  Mat filter;
  float data_45[3][3]={{0,1,0},{1,-4,1},{0,1,0}};
  float data_90[3][3]={{1,1,1},{1,-8,1},{1,1,1}};

  switch (isotropy) {
    case 45:
      filter= Mat(3,3,CV_32F,data_45);
      break;
    case 90:
      filter= Mat(3,3,CV_32F,data_90);
      break;
    default:
      cout<<"Error: Please specify the isotropy of the filter"<<endl;
      exit(0);
  }
    Mat Corr_Convn_result;
    Corr_Conv(image,filter,Corr_Convn_result,0);
    Mat temp_output;
    normalize(Corr_Convn_result,Corr_Convn_result,0,255,NORM_MINMAX,ddepth);
    output=Corr_Convn_result;
}


void Sobel(Mat img,Mat &output,int weightX,int weighty){
  int kernel_dims=3;

  float data_g_x[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};
  Mat g_x= Mat(kernel_dims,kernel_dims,CV_32F,data_g_x)*weightX;

  float data_g_y[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
  Mat g_y = Mat(kernel_dims,kernel_dims,CV_32F,data_g_y)*weighty;
  Mat applied_g_x,applied_g_y;
  Corr_Conv(img,g_x,applied_g_x,0);
  Corr_Conv(img,g_y,applied_g_y,0);
  Mat magnitude;
  Mine_magnitude(applied_g_x,applied_g_y,magnitude);
  magnitude.copyTo(output);
}

void Mine_magnitude(Mat img_g_x,Mat img_g_y,Mat &magnitude){
  Mat pow1,pow2;
  pow(img_g_x,2,pow1);
  pow(img_g_y,2,pow2);
  sqrt((pow1+pow2),magnitude);
  normalize(magnitude,magnitude,0,255,NORM_MINMAX,CV_8UC1);
}
