#include <iostream>
#include <opencv2/opencv.hpp>
#include "./Utility.hpp"

using namespace std;
using namespace cv;

//Using this function to return sharpening/smoothing with laplance/sobel op and Avaraging op
//on the intensity of the image in HSI base. Then I' ll compare the result of those
//ops in RGB based measure.
void RGB2HSV_Smoothing(Mat image,Mat &smoothing,Mat &sharp_laplance,Mat &sharp_sobel,int dim);

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
//Want to work with RGB, by default opencv read image with BGR format. I'll
//convert the format at visualization time.
cvtColor(image,image,COLOR_BGR2RGB);  //Comment this line to work with BGR
Mat RGB[3];
split(image,RGB);   //Splitting the in matrices to work with R,G and B separately.
Mat Sharpened_RGB[3];
Mat Sharpened_RGB_sobel[3];
Mat Smoothed_RGB[3];

//The code for those function can be found in Utility.hpp header file, i will use
//those for calculating the smoothing/sharpening effect.
Laplanceiano(RGB[0],Sharpened_RGB[0],90,Sharpened_RGB[0].type());
Laplanceiano(RGB[1],Sharpened_RGB[1],90,Sharpened_RGB[1].type());
Laplanceiano(RGB[2],Sharpened_RGB[2],90,Sharpened_RGB[2].type());

Sobel(RGB[0],Sharpened_RGB_sobel[0],1,1,Sharpened_RGB_sobel[0].type());
Sobel(RGB[1],Sharpened_RGB_sobel[1],1,1,Sharpened_RGB_sobel[1].type());
Sobel(RGB[1],Sharpened_RGB_sobel[2],1,1,Sharpened_RGB_sobel[2].type());

Avaraging(RGB[0],Smoothed_RGB[0],10);
Avaraging(RGB[1],Smoothed_RGB[1],10);
Avaraging(RGB[2],Smoothed_RGB[2],10);

//After the operations mentioned above I' ll use merge to get back the single matrix
Mat Sum_sharpened_RGB_sobel;
merge(Sharpened_RGB_sobel,3,Sum_sharpened_RGB_sobel);
//Applying the calculated ops to images for the real sharpening effect
Sum_sharpened_RGB_sobel+=image;

Mat Sum_sharpened_RGB_Laplance;
merge(Sharpened_RGB,3,Sum_sharpened_RGB_Laplance);  //Same for the Laplance op
Sum_sharpened_RGB_Laplance+=image;

Mat Sum_Smoothed_RGB;
merge(Smoothed_RGB,3,Sum_Smoothed_RGB);


//Simply displaying the results after this work in RGB based. Note: To correctly
//visualize the results I need to convert the image in BGR format.
cvtColor(Sum_sharpened_RGB_sobel,Sum_sharpened_RGB_sobel,COLOR_RGB2BGR);
namedWindow("Sharpened Sobel RGB: Sum",WINDOW_AUTOSIZE);
imshow("Sharpened Sobel RGB: Sum",Sum_sharpened_RGB_sobel);

cvtColor(Sum_sharpened_RGB_Laplance,Sum_sharpened_RGB_Laplance,COLOR_RGB2BGR);
namedWindow("Sharpened Laplance RGB: Sum",WINDOW_AUTOSIZE);
imshow("Sharpened Laplance RGB: Sum",Sum_sharpened_RGB_Laplance);

cvtColor(Sum_Smoothed_RGB,Sum_Smoothed_RGB,COLOR_RGB2BGR);
namedWindow("Smoothed RGB: Sum",WINDOW_AUTOSIZE);
imshow("Smoothed RGB: Sum",Sum_Smoothed_RGB);

//--------------------

Mat smoothing_HSV,sharp_laplance_HSV,sharp_sobel_HSV;
//Using this function to return the work of: Sharpening with Laplance,Sobel op and
//smoothing with avarage op on the intensity value in HSI format. Code below
RGB2HSV_Smoothing(image,smoothing_HSV,sharp_laplance_HSV,sharp_sobel_HSV,10);

//Again, Simply displaying the image after an RGB to BGR convertion
cvtColor(smoothing_HSV,smoothing_HSV,COLOR_RGB2BGR);
namedWindow("Smoothed RGB with HSV",WINDOW_AUTOSIZE);
imshow("Smoothed RGB with HSV",smoothing_HSV);

cvtColor(sharp_laplance_HSV,sharp_laplance_HSV,COLOR_RGB2BGR);
namedWindow("Sharpened RGB with HSV Laplance",WINDOW_AUTOSIZE);
imshow("Sharpened RGB with HSV Laplance",sharp_laplance_HSV);

cvtColor(sharp_sobel_HSV,sharp_sobel_HSV,COLOR_RGB2BGR);
namedWindow("Sharpened RGB with HSV Sobel",WINDOW_AUTOSIZE);
imshow("Sharpened RGB with HSV Sobel",sharp_sobel_HSV);


waitKey(0);
return 0;
}


void RGB2HSV_Smoothing(Mat image,Mat &smoothing,Mat &sharp_laplance,Mat &sharp_sobel,int dim){
  //Conversion in HSV image
  Mat ImageHSV;
  cvtColor(image,ImageHSV,COLOR_BGR2HSV);
  //Need to declare those variables below to not make every trasformation work on
  //the original value.
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

  //Merging the results of those trasformation and copying them into the position pointed,
  //of course after converting them back in RGB (after in BGR).
  Mat image_smoothed;
  merge(hsv_smooth,3,image_smoothed);
  cvtColor(image_smoothed,image_smoothed,COLOR_HSV2BGR);
  image_smoothed.copyTo(smoothing);

  Mat image_sharped_Laplance;
  merge(hsv_sharp_Laplance,3,image_sharped_Laplance);
  cvtColor(image_sharped_Laplance,image_sharped_Laplance,COLOR_HSV2BGR);
  image_sharped_Laplance+=image;
  image_sharped_Laplance.copyTo(sharp_laplance);

  Mat image_sharped_Sobel;
  merge(hsv_sharp_Sobel,3,image_sharped_Sobel);
  cvtColor(image_sharped_Sobel,image_sharped_Sobel,COLOR_HSV2BGR);
  image_sharped_Sobel+=image;
  image_sharped_Sobel.copyTo(sharp_sobel);

}
