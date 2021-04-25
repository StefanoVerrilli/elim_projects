#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void Laplanceiano(Mat image,Mat &output,int isotropy,int ddepth);
Mat Convolution(Mat img,Mat filter);

int main(int argc,char**argv){

if(argc<2){
  cout<<"Usage: "<<argv[0]<<"filename.ext"<<endl;
  return -1;
}

Mat image = imread(argv[1],IMREAD_GRAYSCALE);
if(image.empty()){
  cout<<"Error: image not found"<<endl;
  return -1;
}

  Mat laplanceiano_output_45;
  Laplanceiano(image,laplanceiano_output_45,45,laplanceiano_output_45.type());

  Mat laplanceiano_output_90;
  Laplanceiano(image,laplanceiano_output_90,90,laplanceiano_output_90.type());

  namedWindow("Laplancian result 45 degree",WINDOW_AUTOSIZE);
  imshow("Laplancian result 45 degree",laplanceiano_output_45);

  namedWindow("Laplancian result 90 degree",WINDOW_AUTOSIZE);
  imshow("Laplancian result 90 degree",laplanceiano_output_90);

  waitKey(0);
  return 0;


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
  cout<<"filter applied:\n"<<filter<<endl;
    Mat convolution_result = Convolution(image,filter);
    Mat temp_output;
    normalize(convolution_result,convolution_result,0,255,NORM_MINMAX,ddepth);
    output=convolution_result;
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
