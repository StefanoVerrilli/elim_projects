#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

const double th = 15.8;
const int tsize = 4;

class TNode{
    private:
        Rect R;
        TNode *UL,*LL,*UR,*LR;
        vector<TNode *> merged;
        vector<bool> mergedB = vector<bool>(4,false);
        double stdev,mean;
    public:
        TNode(Rect R){this->R = R; UL=LL=UR=LR=nullptr;}
        TNode * getUL(){return UL;}
        TNode * getLL(){return LL;}
        TNode * getUR(){return UR;}
        TNode * getLR(){return LR;}
        double getMean(){return mean;}
        double getSTD(){return stdev;}
        void setMean(double mean){this->mean = mean;}
        void setSTD(double std){this->stdev = std;}
        void setUL(TNode * UL){this->UL = UL;}
        void setLL(TNode * LL){this->LL = LL;}
        void setUR(TNode * UR){this->UR = UR;}
        void setLR(TNode * LR){this->LR = LR;}
        void addRegion(TNode * R){this->merged.push_back(R);}
        vector<TNode *>getMerged(){return merged;}
        Rect getRegion(){return R;}
        void setMerged(int i){this->mergedB[i] = true;}
        bool getMerged(int i){return mergedB[i];}
};
TNode * split(Mat &img,Rect R);

void merge(TNode *root){
    if(root->getRegion().width > tsize && root->getSTD() > th){
        if(root->getUL()->getSTD() <= th && root->getUR()->getSTD() <= th){
            root->addRegion(root->getUL());
            root->addRegion(root->getUR());
            root->setMerged(0);
            root->setMerged(1);
        if(root->getLL()->getSTD()<=th && root->getLR()->getSTD()<=th){
            root->addRegion(root->getLL());
            root->addRegion(root->getLR());
            root->setMerged(2);
            root->setMerged(3);
        }else{
            merge(root->getLL());
            merge(root->getLR());
        }
        }else if(root->getUR()->getSTD() <= th &&  root->getLR()->getSTD()<= th){
            root->addRegion(root->getUR());
            root->addRegion(root->getLR());
            root->setMerged(1);
            root->setMerged(2);
            if(root->getLL()->getSTD()<=th && root->getUL()->getSTD()<= th){
                root->addRegion(root->getLL());
                root->addRegion(root->getUL());
                root->setMerged(3);
                root->setMerged(0);
            }else{
                merge(root->getLL());
                merge(root->getUL());
            }
        }else if(root->getLL()->getSTD()<= th && root->getUL()->getSTD()<= th){
            root->addRegion(root->getLL());
            root->addRegion(root->getUL());
            root->setMerged(3);
            root->setMerged(0);
            if(root->getUR()->getSTD()<= th && root->getLR()->getSTD()<= th){
                root->addRegion(root->getUR());
                root->addRegion(root->getLR());
                root->setMerged(1);
                root->setMerged(2);
            }else{
                merge(root->getUR());
                merge(root->getLR());
            }
        }else if(root->getLR()->getSTD()<= th && root->getLL()->getSTD()<= th){
            root->addRegion(root->getLR());
            root->addRegion(root->getLL());
            root->setMerged(2);
            root->setMerged(3);
            if(root->getUR()->getSTD() <= th && root->getUL()->getSTD()<= th){
                root->addRegion(root->getUR());
                root->addRegion(root->getUL());
                root->setMerged(0);
                root->setMerged(1);
            }else{
                merge(root->getUR());
                merge(root->getUL());
            }
        }else{
            merge(root->getLL());
            merge(root->getLR());
            merge(root->getUL());
            merge(root->getUR());
        }
    }else{
        root->addRegion(root);root->setMerged(0);root->setMerged(1);root->setMerged(2);root->setMerged(3);
    }

}

TNode * split(Mat &img,Rect R){

    TNode* region = new TNode(R);

    Scalar mean,STD;
    cv::meanStdDev(img(R),mean,STD);

    region->setSTD(STD[0]);
    region->setMean(mean[0]);

    if(R.width > tsize && region->getSTD() > th){

        Rect UL(R.x,R.y,R.height/2,R.width/2);
        region->setUL(split(img,UL));

        Rect UR(R.x,R.y+R.width/2,R.height/2,R.width/2);
        region->setUR(split(img,UR));

        Rect LL(R.x+R.height/2,R.y,R.height/2,R.width/2);
        region->setLL(split(img,LL));

        Rect LR(R.x+R.height/2,R.y+R.width/2,R.height/2,R.width/2);
        region->setLR(split(img,LR));
    }
    rectangle(img,R,Scalar(0));
    return region;

}

void segment(TNode * root,Mat &img){

    vector<TNode *> Regions = root->getMerged();

    if(!Regions.size()){
        segment(root->getUL(),img);
        segment(root->getLL(),img);
        segment(root->getUR(),img);
        segment(root->getLR(),img);
    }else{
        double value = 0;
        for(int i =0;i<Regions.size();i++){
            value+= (int)Regions.at(i)->getMean();
        }
        value/=Regions.size();
        for(auto x:Regions){
            img(x->getRegion()) = (int)value;
        }
        if(Regions.size() > 1){
            if(!root->getMerged(0))
                segment(root->getUL(),img);
            if(!root->getMerged(1))
                segment(root->getUR(),img);
            if(!root->getMerged(2))
                segment(root->getLR(),img);
            if(!root->getMerged(3))
                segment(root->getLL(),img);
        }
    }
}


int main(int argc,char** argv){
    Mat img = imread(argv[1],IMREAD_GRAYSCALE);
    if(img.empty()){
        return -1;
    }
    Mat BlurredImage;
    GaussianBlur(img,BlurredImage,Size(5,5),0,0);
    //resize(BlurredImage,BlurredImage,Size(0,0),0.5,0.5);
    /*int exp = log(min(BlurredImage.cols,BlurredImage.rows))/ log(2);
    int s = pow(2.0,(double)exp);
    Rect square = Rect(0,0,s,s);
    BlurredImage = BlurredImage(square).clone();
    Mat ImageSeg = BlurredImage.clone();*/
    TNode * root = split(BlurredImage,Rect(0,0,BlurredImage.rows,BlurredImage.cols));
    merge(root);
    segment(root,BlurredImage);
    imshow("original",BlurredImage);
    //imshow("output",ImageSe);
    waitKey();
    return 0;

}