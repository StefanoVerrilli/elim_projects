#include <iostream>
#include <opencv2/opencv.hpp>
#include "./Utility.hpp"

using namespace std;
using namespace cv;

//Using this function to return sharpening/smoothing with laplance/sobel op and Avaraging op
//on the intensity of the image in HSI base. Then I' ll compare the result of those
//ops in BRG based measure.
void BRG2HSV_Smoothing(Mat image,Mat &smoothing,Mat &sharp_laplance,Mat &sharp_sobel,int dim);

int main(int argc,char**argv){

if(argc<2){
  cout<<"Usage:"<<argv[0]<<"image"<<endl;
  exit(0);
}
Mat image = imread(argv[1],IMREAD_COLOR);
if(image.empty()){
  cout<<"Error in reading image"<<endl;
  return -1;
}

//Note: Opencv use the BGR format and not the BRG format, for the sake of this
//code I'll work with this specific notation.

Mat BRG[3];
split(image,BRG);   //Splitting the in matrices to work with R,G and B separately.
Mat Sharpened_BRG[3];
Mat Sharpened_BRG_sobel[3];
Mat Smoothed_BRG[3];

//The code for those function can be found in Utility.hpp header file, i will use
//those for calculating the smoothing/sharpening effect.
Laplanceiano(BRG[0],Sharpened_BRG[0],90,Sharpened_BRG[0].type());
Laplanceiano(BRG[1],Sharpened_BRG[1],90,Sharpened_BRG[1].type());
Laplanceiano(BRG[2],Sharpened_BRG[2],90,Sharpened_BRG[2].type());

Sobel(BRG[0],Sharpened_BRG_sobel[0],1,1,Sharpened_BRG_sobel[0].type());
Sobel(BRG[1],Sharpened_BRG_sobel[1],1,1,Sharpened_BRG_sobel[1].type());
Sobel(BRG[1],Sharpened_BRG_sobel[2],1,1,Sharpened_BRG_sobel[2].type());

Avaraging(BRG[0],Smoothed_BRG[0],10);
Avaraging(BRG[1],Smoothed_BRG[1],10);
Avaraging(BRG[2],Smoothed_BRG[2],10);

//After the operations mentioned above I' ll use merge to get back the single matrix
Mat Sum_sharpened_BRG_sobel;
merge(Sharpened_BRG_sobel,3,Sum_sharpened_BRG_sobel);
//Applying the calculated ops to images for the real sharpening effect
Sum_sharpened_BRG_sobel+=image;

Mat Sum_sharpened_BRG_Laplance;
merge(Sharpened_BRG,3,Sum_sharpened_BRG_Laplance);  //Same for the Laplance op
normalize(Sum_sharpened_BRG_Laplance,Sum_sharpened_BRG_Laplance,0,255,NORM_MINMAX,CV_8U);
float c=-1;
Sum_sharpened_BRG_Laplance+=(c*image);

Mat Sum_Smoothed_BRG;
merge(Smoothed_BRG,3,Sum_Smoothed_BRG);


//Simply displaying the results after this work in BGR based.

namedWindow("Sharpened Sobel BRG: Sum",WINDOW_AUTOSIZE);
imshow("Sharpened Sobel BRG: Sum",Sum_sharpened_BRG_sobel);

namedWindow("Sharpened Laplance BRG: Sum",WINDOW_AUTOSIZE);
imshow("Sharpened Laplance BRG: Sum",Sum_sharpened_BRG_Laplance);

namedWindow("Smoothed BRG: Sum",WINDOW_AUTOSIZE);
imshow("Smoothed BRG: Sum",Sum_Smoothed_BRG);

//--------------------

Mat smoothing_HSV,sharp_laplance_HSV,sharp_sobel_HSV;
//Using this function to return the work of: Sharpening with Laplance,Sobel op and
//smoothing with avarage op on the intensity value in HSI format. Code below
BRG2HSV_Smoothing(image,smoothing_HSV,sharp_laplance_HSV,sharp_sobel_HSV,100);

//Again, Simply displaying the images returned
namedWindow("Smoothed BRG with HSV",WINDOW_AUTOSIZE);
imshow("Smoothed BRG with HSV",smoothing_HSV);

namedWindow("Sharpened BRG with HSV Laplance",WINDOW_AUTOSIZE);
imshow("Sharpened BRG with HSV Laplance",sharp_laplance_HSV);

namedWindow("Sharpened BRG with HSV Sobel",WINDOW_AUTOSIZE);
imshow("Sharpened BRG with HSV Sobel",sharp_sobel_HSV);


waitKey(0);
return 0;
}


void BRG2HSV_Smoothing(Mat image,Mat &smoothing,Mat &sharp_laplance,Mat &sharp_sobel,int dim){
  //Conversion in HSV image
  Mat ImageHSV;
  cvtColor(image,ImageHSV,COLOR_BGR2HSV);
  //Need to declare those variables below not alter the original value every op
  Mat hsv_sharp_Laplance[3],hsv_sharp_Sobel[3],hsv_smooth[3];
  //Splitting the HSV matrix in hue, saturation and intensity
  split(ImageHSV,hsv_smooth);
  split(ImageHSV,hsv_sharp_Sobel);
  split(ImageHSV,hsv_sharp_Laplance);

  //Applying the Avaraging, Sobel and Laplance filter to the intensity
  //of the HSV_image (utility.hpp)
  Avaraging(hsv_smooth[2],hsv_smooth[2],10);

  Laplanceiano(hsv_sharp_Laplance[2],hsv_sharp_Laplance[2],90,hsv_sharp_Laplance[2].type());

  Sobel(hsv_sharp_Sobel[2],hsv_sharp_Sobel[2],1,1,hsv_sharp_Sobel[2].type());

  Mat image_smoothed;
  merge(hsv_smooth,3,image_smoothed);
  cvtColor(image_smoothed,image_smoothed,COLOR_HSV2BGR);
  image_smoothed.copyTo(smoothing);

  Mat image_sharped_Laplance;
  merge(hsv_sharp_Laplance,3,image_sharped_Laplance);
  cvtColor(image_sharped_Laplance,image_sharped_Laplance,COLOR_HSV2BGR);
  normalize(image_sharped_Laplance,image_sharped_Laplance,0,255,NORM_MINMAX,CV_8U);
  float c=-1;
  image_sharped_Laplance+=(c*image);
  image_sharped_Laplance.copyTo(sharp_laplance);

  Mat image_sharped_Sobel;
  merge(hsv_sharp_Sobel,3,image_sharped_Sobel);
  cvtColor(image_sharped_Sobel,image_sharped_Sobel,COLOR_HSV2BGR);
  image_sharped_Sobel+=image;
  image_sharped_Sobel.copyTo(sharp_sobel);

}
