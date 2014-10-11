#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"


#include "cv.h"  
#include "highgui.h"  
#include <ml.h>  
#include <iostream>  
#include <fstream>  
#include <string>  
#include <vector>  

//#include <stdio.h>
//#include <string.h>
//#include <ctype.h>


using namespace cv;
using namespace std;
#define THRESHOLD 20

int myscale = 2;

//获取图像的感兴趣区域，返回矩形区域的rect,
//输入:src为二值图像
//输出:返回CvRect
CvRect getROI_1(IplImage* src);
//获取图像的感兴趣区域，返回矩形区域的rect,
//输入:src为二值图像
//输出:返回CvRect
CvRect getROI_2(IplImage *img);
//使用帧差，获取检测后的二值图像
//输入:两幅灰度图
//输出:使用帧差法之后的二值图像
void getBW(IplImage* currFrame, IplImage * preFrame ,IplImage *dst);
CvRect getDetectRegion(IplImage* currFrame, IplImage * preFrame, int expandRect, bool Mode);
DetectionROI rect2DetectionROI(cv::Rect ROI,cv::Size stride,cv::Size window);


int main(int argc, char** argv)
{
	IplImage m1,m2;
	// Mat img;
	FILE* f = 0;
	char _filename[1024];
	int cur_frame;
	cv::Rect rect;
	
	FILE *fin_detector = fopen("../detector/default_detector6464.txt","r");
	float temp;
	char c;
	vector<float> mydescriptors;//3781 
	while(!feof(fin_detector))
	{
		fscanf(fin_detector,"%f",&temp);//fin_detector >> temp;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		mydescriptors.push_back(temp);
	}
	fclose(fin_detector);
	cout<<"size of detector is  "<<mydescriptors.size()<<endl;

	VideoCapture cap(argv[1]);
		//	CvCapture* tap = cvCreateFileCapture("a.avi");  
		if (!cap.isOpened())  
		{  
			printf("avi file load error...");  
				system("pause");  
				exit(-1);  
		}  
	vector<cv::Rect>  found;  
	vector<DetectionROI> ROIs;
	cv::HOGDescriptor hog(cv::Size(64,64), cv::Size(16,16), cv::Size(8,8), cv::Size(8,8), 9);  
	hog.setSVMDetector(mydescriptors); 
	//hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	IplImage* img = NULL;  
	// cvNamedWindow("img", 0);
	//Mat dstimg;
	//dstimg= cvCreateMat(64*2,128*2,1);
	Mat matimg;
	//Mat matimg2;
	Mat mat_original;
	//while(img=cvQueryFrame(cap))  
	Point ptlt,ptrb;//左上、右下;
	//cur_frame=0;
	while(cap.read( mat_original))
	{  
		////////////////////////////test a image
		//matimg = imread("0001.jpg");
		//cvShowImage("img",img);
		//imshow("image",matimg);
		//resize(matimg,dstimg,Size(64,128));
		// cv::Rect rect=cvRect(0,0,matimig.cols,matimg.rows);
		resize(mat_original,mat_original,Size((int)(-mat_original.cols%myscale+mat_original.cols)/myscale,(int)(-mat_original.rows%myscale+mat_original.rows)/myscale));
		mat_original.copyTo(matimg);

		/*
		if(cur_frame==0)  rect=cvRect(0,0,mat_original.cols,mat_original.rows);
		else
		{
			m1=IplImage(matimg);
			m2=IplImage(matimg2);
			rect=getDetectRegion(&m1,&m2,50,1);
		}
		*/
		rect.x = 0;
		rect.y = 0;
		rect.width = matimg.cols;
		rect.height = matimg.rows;
		DetectionROI ROI=rect2DetectionROI(rect,cv::Size(8,8),cv::Size(64,64));
		ROIs.push_back(ROI);
		//matimg2=matimg.clone();
		// ROIs.push_back(rect);
		//hog.detectMultiScaleROI(matimg, found,ROIs );  
		// ROIs.clear();
		//cvWaitKey(500);
		//hog.detectMultiScale(matimg, found, 0, cv::Size(8,8), cv::Size(4,4), 1.2, 2);  
		hog.detectMultiScaleROI(matimg, found, ROIs,0,0);  
		if (found.size() > 0){ 
			//printf("pedestrian detected is %d\n",found.size());
			for (int i=0; i<found.size(); i++){  
				rectangle(matimg, Point(found[i].x,found[i].y),Point(found[i].x+found[i].width,found[i].y+found[i].height),CV_RGB(255,0,0), 2);  
			}  
		}  
		imshow("image",matimg);
		//imwrite("0404105.jpg",matimg);
		cvWaitKey(100);
		ROIs.clear();
	}  
	cap.release(); 
	return 0;
}

CvRect getROI_1(IplImage* src)
{
	int width=src->width;
	int height=src->height;

	int x1=0,y1=0; //左上角坐标
	int x2=width-1, y2=height-1;//左下角坐标

	bool up=false;
	for(int i=0;i<height;i++)
	{
		uchar* ptr=(uchar*)(src->imageData+src->widthStep*i);
		for(int j=0;j<width;j++)
		{
			if(ptr[j]!=0)
			{
				up=true;
				break;
			}
		}
		if(up){y1=i;break;}   //获取y1的坐标
	}

	bool down=false;
	for(int i=height-1;i>=0;i--)
	{
		uchar* ptr=(uchar*)(src->imageData+src->widthStep*i);
		for(int j=0;j<width;j++)
		{
			if(ptr[j]!=0)
			{
				down=true;
				break;
			}
		}
		if(down){y2=i;break;}  //获取y2的坐标
	}

	bool left=false;int temp;
	for(int i=0;i<width;i++)
	{
		for(int j=y1;j<=y2;j++)
		{
			temp=((uchar* )(src->imageData+src->widthStep*j))[i];
			if(temp!=0)
			{
				left=true;
				break;
			}
		}
		if(left){x1=i;break;}   //获取x1的坐标
	}

	bool right=false;
	for(int i=width-1;i>=0;i--)
	{
		for(int j=y1;j<=y2;j++)
		{
			temp=((uchar* )(src->imageData+src->widthStep*j))[i];
			if(temp!=0)
			{
				right=true;
				break;
			}
		}
		if(right){x2=i;break;} //获取x2的坐标
	}

	return cvRect(x1,y1,x2-x1+1,y2-y1+1);
}


CvRect getROI_2(IplImage *img)
{
	IplImage* src=cvCloneImage(img);
	CvSeq* seq;

	vector<CvRect> boxes;

	CvMemStorage* storage = cvCreateMemStorage(0);
	cvClearMemStorage(storage);

	cvFindContours(src, storage, &seq, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	CvRect boundbox ;

	for(; seq; seq = seq->h_next) {
		boundbox = cvBoundingRect(seq);
		boxes.push_back(boundbox);
	}

	int xMin=src->width-1,yMin=src->height-1; 
	int xMax=0,yMax=0;
	for(int i=0;i<boxes.size();i++)
	{
		CvRect rect=boxes[i];
		int x1=rect.x, y1=rect.y;  //左上角坐标
		int x2=x1+rect.width-1, y2=y1+rect.height-1; //右下角坐标
		if(x1<xMin){xMin=x1;}
		if(y1<yMin){yMin=y1;}
		if(x2>xMax){xMax=x2;}
		if(y2>yMax){yMax=y2;}
	}
	cvReleaseImage(&src);
	cvReleaseMemStorage(&storage);
	return cvRect(xMin, yMin, xMax-xMin+1, yMax-yMin+1);
}

//使用帧差，获取检测后的二值图像
//输入:两幅灰度图
//输出:使用帧差法之后的二值图像
void getBW(IplImage* currFrame, IplImage * preFrame ,IplImage *dst)
{
	IplImage *&diff=dst;
	cvSub(currFrame,preFrame,diff);
	cvAbs(diff,diff);   //两帧相减求取绝对值

	cvThreshold(diff,diff,THRESHOLD,255.0,CV_THRESH_BINARY);

}


CvRect getDetectRegion(IplImage* currFrame, IplImage * preFrame, int expandRect, bool Mode)
{
	if(expandRect < 0)
		expandRect = 0;


	IplImage* currF = cvCreateImage(cvGetSize(currFrame),currFrame->depth ,1);
	IplImage* preF = cvCreateImage(cvGetSize(preFrame),preFrame->depth,1);

	IplImage* bw=cvCreateImage(cvGetSize(currFrame),currFrame->depth,1);

	cvCvtColor(currFrame,currF,CV_BGR2GRAY);
	cvCvtColor(preFrame,preF,CV_BGR2GRAY);

	getBW(currF,preF,bw);   //获取二值图像

	IplConvKernel* element = cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_RECT,0);//创建结构元素  

	cvErode(bw,bw,element,1);//腐蚀图像
	cvDilate(bw,bw,element,1);//膨胀图像  

	CvRect rect;

	if(Mode)
		rect=getROI_1(bw); //收缩矩形
	else
		rect=getROI_2(bw); //寻找序列

	if(expandRect > 0)
	{
		int width = bw->width;
		int height = bw->height;

		//右下角坐标
		int x=rect.x + rect.width-1;  x+=expandRect; if(x>=width) x=width-1;
		int y=rect.y + rect.height-1; y+=expandRect; if(y>=height)y=height-1;

		//左上角坐标
		rect.x-=expandRect ; if(rect.x<0)rect.x=0;
		rect.y-=expandRect ; if(rect.y<0)rect.y=0;

		rect.width =x- rect.x+1;
		rect.height =y- rect.y+1;

	}

	if(1)
	{
		cvRectangle(currFrame,cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width-1,rect.y+rect.height-1),cvScalar(0x00,0xff,0xff),2);
	}
	cvReleaseImage(&currF);
	cvReleaseImage(&preF);
	cvReleaseImage(&bw);

	cvReleaseStructuringElement(&element);

	return rect;
}
DetectionROI rect2DetectionROI(cv::Rect ROI,cv::Size stride,cv::Size window)
{
	DetectionROI detectedROI;
	detectedROI.scale=2;
	cv::Point point;
	int basex=ROI.x;
	int basey=ROI.y;
	while(basex%stride.width!=0)basex++;
	while(basey%stride.height!=0)basey++;
	int m=ROI.x+ROI.width-window.width;
	int n=ROI.y+ROI.height-window.height;
	for(int i=basex;i<m;i+=stride.width)
	{
		for(int j=basey;j<n;j+=stride.height)
		{
			point.x=i;
			point.y=j;
			detectedROI.locations.push_back(point);
		}
	}
	return detectedROI;
}

