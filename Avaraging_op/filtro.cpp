#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat Convoluzione(Mat image,Mat filter);
int main( int argc,char**argv){
  if(argc<2){
      cout<<"usage: " << argv[0]<< " image_name"<<endl;
      exit(0);
  }
  //Leggo l' immagine da linea di comando
  Mat image = imread(argv[1], IMREAD_GRAYSCALE);
  if(image.empty() ){
      cout<<"Errore in lettura, immagine non trovata"<<std::endl;
      return -1;
  }

  //Copio l'immagine per non effettuare modifiche sull' immagine originale
  Mat imageCorrelation;
  Mat imageConvolution;
  Mat padded_img;

  int dim=10;

  Mat Conv_Mean_filter;
  Mat Corr_Mean_filter=Mat::ones(dim,dim,CV_32F)/(float) (dim*dim);

  rotate(Corr_Mean_filter,Conv_Mean_filter,ROTATE_180);

  imageCorrelation = Convoluzione(image,Corr_Mean_filter);
  imageConvolution = Convoluzione(image,Conv_Mean_filter);

  imageCorrelation.convertTo(imageCorrelation,CV_32F,1.0/255,0);
  imageConvolution.convertTo(imageConvolution,CV_32F,1.0/255,0);

  namedWindow("Original image",WINDOW_AUTOSIZE);
  imshow("Original image",image);

  Mat filter2D_result;
  filter2D(image,filter2D_result,image.type(),Corr_Mean_filter);
  namedWindow("filter2D result",WINDOW_AUTOSIZE);
  imshow("filter2D result",filter2D_result);

  namedWindow("Correlation with Mean_filter applied img",WINDOW_AUTOSIZE);
  imshow("Correlation with Mean_filter applied img",imageCorrelation);

  namedWindow("Convolution with Mean_filter applied img",WINDOW_AUTOSIZE);
  imshow("Convolution with Mean_filter applied img",imageConvolution);

  waitKey(0);
  destroyAllWindows();
  return 0;
}



Mat Convoluzione(Mat image,Mat filter){
  Mat Correlation_image(image.rows,image.cols,filter.type());
  Mat padded_img;
  int pad=filter.cols/2;
  float filtertotal=0.F;
  copyMakeBorder(image,padded_img,pad,pad,pad,pad,BORDER_CONSTANT,Scalar(0));
  padded_img.convertTo(padded_img,filter.type());
  for(int conv_rows=0;conv_rows<image.rows;conv_rows++){
    for(int conv_cols=0;conv_cols<image.cols;conv_cols++){
        filtertotal=0.F;
        Rect r(Point(conv_cols,conv_rows),Point(conv_cols+filter.cols,conv_rows+filter.rows));
        Mat block = padded_img(r);
        //cout<<block<<endl;
        Mat product = block.mul(filter);
        for(int block_rows=0;block_rows<filter.rows;block_rows++){
          for(int block_cols=0;block_cols<filter.cols;block_cols++){
            filtertotal+=product.at<float>(block_rows,block_cols);
          }
        }
        Correlation_image.at<float>(conv_rows,conv_cols)=filtertotal;
        //image.at<unsigned char>(conv_rows,conv_cols)=filtertotal;
    }
  }
  return Correlation_image  ;

}
