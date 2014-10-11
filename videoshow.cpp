
#include <highgui.h>   
#include <iostream>   
using namespace cv;  
using namespace std;  
int main(int argc,char **argv)  
    {  
        cvNamedWindow("Test",CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture(argv[1]);
	if(!capture)
	{
		cout<<"fail to load file"<<endl;
		return 1;
	}
	IplImage* frame;
	while(1)
	{
		frame = cvQueryFrame(capture);
		if(!frame)
		{
			cout<<"error,fail to  read file"<<endl;
			break;

		}
		cvShowImage("Test",frame);
		cvWaitKey(10);
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Test");
	return 0;
    }  
