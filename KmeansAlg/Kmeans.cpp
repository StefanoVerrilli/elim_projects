#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void InizializeCentroids(Mat img,int ClusterNumeber,vector<vector<Point>> &ptInCluster,vector<Scalar> &centroids);
double Distance(Scalar pt,Scalar ClusterCentroid);
void AssignToCluster(Mat img,int ClusterNumber, vector<vector<Point>> &ptInCluster,vector<Scalar> centroids);
double MoveCentroids(Mat img,int ClusterNumber,vector<vector<Point>> &ptInCluster,vector<Scalar> &centroids,double &oldCenter,double &newCenter);
void ApplyClustering(Mat &img,int ClusterNumber,vector<vector<Point>> &ptInCluster,vector<Scalar> &centroids);


double Distance(Scalar pt,Scalar ClusterCentroid){

    double bluediff = pt.val[0] - ClusterCentroid.val[0];
    double greendiff = pt.val[1] - ClusterCentroid.val[1];
    double reddiff = pt.val[2] - ClusterCentroid.val[2];

    double diff = sqrt(pow(bluediff,2) + pow(greendiff,2) + pow(reddiff,2));

    return diff;
}

void InizializeCentroids(Mat img,int ClusterNumber,vector<vector<Point>> &ptInCluster,vector<Scalar> &centroids){
RNG random(cv::getTickCount());

    for(int i=0;i<ClusterNumber;i++){
        Point centerForCluster;
        centerForCluster.x = random.uniform(0,img.cols);
        centerForCluster.y = random.uniform(0,img.rows);
        Scalar centroidPixel = img.at<Vec3b>(centerForCluster.y,centerForCluster.x);

        centroids.push_back(centroidPixel);

        vector<Point> PointsInCluster;
        ptInCluster.push_back(PointsInCluster);
    }
}

void AssignToCluster(Mat img,int ClusterNumber, vector<vector<Point>> &ptInCluster,vector<Scalar> centroids){
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            Scalar pixel = img.at<Vec3b>(i,j);
            int indexMinDistanceCluster = 0;
            double minDistance = INFINITY;
            for(int k=0;k<ClusterNumber;k++){
                Scalar ClusterCenter = centroids[k];
                double distance = Distance(pixel,ClusterCenter);
                if(distance < minDistance){
                    minDistance = distance;
                    indexMinDistanceCluster = k;
                }
            }
            ptInCluster[indexMinDistanceCluster].push_back(Point(j,i));
        }
    }
}


double MoveCentroids(Mat img,int ClusterNumber,vector<vector<Point>> &ptInCluster,vector<Scalar> &centroids,double &oldCenter,double &newCenter){
    double diffMovement;
    for(int k=0;k<ClusterNumber;k++){
        vector<Point> ptSingleCluster = ptInCluster.at(k);
        double newBlue =0 ;
        double newGreen=0;
        double newRed=0;
        for(int i=0;i<ptSingleCluster.size();i++){
            Scalar pixel = img.at<Vec3b>(ptSingleCluster[i].y,ptSingleCluster[i].x);
            newBlue += pixel.val[0];
            newGreen +=pixel.val[1];
            newRed += pixel.val[2];
        }

        newBlue /= ptSingleCluster.size();
        newGreen /= ptSingleCluster.size();
        newRed /= ptSingleCluster.size();

        Scalar newPixel(newBlue,newGreen,newRed);

        newCenter += Distance(newPixel,centroids[k]);
        centroids[k] = newPixel;
    }
    newCenter /= ClusterNumber;
    diffMovement = abs(newCenter - oldCenter);
    oldCenter = newCenter;
    return diffMovement;
}


void ApplyClustering(Mat &img,int ClusterNumber,vector<vector<Point>> &ptInCluster,vector<Scalar> &centroids){
    for(int i=0;i<ClusterNumber;i++){
        vector<Point> ptInSingleCluster = ptInCluster[i];
        for(int k=0;k<ptInSingleCluster.size();k++){
            Scalar centroid = centroids[i];
            img.at<Vec3b>(ptInSingleCluster[k])[0] = centroid.val[0];
            img.at<Vec3b>(ptInSingleCluster[k])[1] = centroid.val[1];
            img.at<Vec3b>(ptInSingleCluster[k])[2] = centroid.val[2]; 
        }
    }
}



int main(int argc,char** argv){
    
    Mat img = imread(argv[1]);
    if(img.empty()){
        return -1;
    }

    int ClusterNum = stoi(argv[2]);
    double th = 0.1;
    vector<vector<Point>> ptInCluster;
    vector<Scalar> centroids;
    double oldCenter = INFINITY;
    double newCenter = 0;
    double diff = oldCenter - newCenter;
    InizializeCentroids(img,ClusterNum,ptInCluster,centroids);
    while(diff > th){
        newCenter = 0;
        for(int k=0;k<ClusterNum;k++){
            ptInCluster[k].clear();
        }
        AssignToCluster(img,ClusterNum, ptInCluster,centroids);
        diff = MoveCentroids(img,ClusterNum,ptInCluster,centroids,oldCenter,newCenter);
    }
    ApplyClustering(img,ClusterNum,ptInCluster,centroids);
    imshow("output",img);
    waitKey();
    return 0;
}