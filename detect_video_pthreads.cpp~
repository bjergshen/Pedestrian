#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <time.h>



using namespace cv;
using namespace std;

void *frame1(void *arg);
void *frame2(void *arg);
void *frame3(void *arg);
void *frame4(void *arg);


int hardExampleCount = 0; 
int gethardexaple = 0;
int scale = 1;

unsigned int i,j;
 
vector<float> myDetector;//3781 
HOGDescriptor hog(cvSize(64,64),cvSize(16,16),cvSize(8,8),cvSize(8,8),9);  

double getTimeval(){
	struct rusage stRusage;
	struct timeval stTimeval;
//	gettimeofday(&stTimeval, NULL);
	getrusage(RUSAGE_SELF, &stRusage);
	stTimeval = stRusage.ru_utime; 
 
	return stTimeval.tv_sec + (double)stTimeval.tv_usec*1E-6; 
}


VideoCapture capture;//(argv[1]);
Mat frame;

int main(int argc,char** argv )
{	
	//detector data read;
	FILE *fin_detector = fopen("../detector/default_detector6464.txt","r");
	float temp;
	char c;
	while(!feof(fin_detector))
	{
		fscanf(fin_detector,"%f",&temp);//fin_detector >> temp;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		myDetector.push_back(temp);
	}
	
	hog.setSVMDetector(myDetector);
	

	double t_start = (double)getTimeval();
	double t_end;
		

	//Threads lines:
	pthread_t id0,id1, id2,id3;
	
	int flag_thread;
	flag_thread = pthread_create(&id0,NULL,frame1,(void *)argv[1]);
	if(!flag_thread){
		printf("fail to create thread 0\n");
	}
	
	flag_thread = pthread_create(&id1,NULL,frame2,(void *)argv[1]);
	if(!flag_thread){
		printf("fail to create thread 1\n");
	}
	
	flag_thread = pthread_create(&id2,NULL,frame3,(void *)argv[1]);
	if(!flag_thread){
		printf("fail to create thread 2\n");
	}
	
	flag_thread = pthread_create(&id3,NULL,frame4,(void *)argv[1]);
	if(!flag_thread){
		printf("fail to create thread 3\n");
	}

	flag_thread = pthread_join(id0, NULL);
	if(!flag_thread){
		printf("thread 0 exit !\n");
	}	
	flag_thread = pthread_join(id1, NULL);
	if(!flag_thread){
		printf("thread 1 exit !\n");
	}	
	flag_thread = pthread_join(id2, NULL);
	if(!flag_thread){
		printf("thread 2 exit !\n");
	}	
	flag_thread = pthread_join(id3, NULL);
	if(!flag_thread){
		printf("thread 3 exit !\n");
	}	
	
	t_end = (double)getTimeval();
	printf("time is    %10.70lf \n",(double)(t_end - t_start));
	fclose(fin_detector);
	return 0;
}

void *frame1(void *arg){
	printf("0");
	char *argv;
	argv = (char *)arg;
	VideoCapture capture;//(argv[1]);
	Mat frame;
	capture.open(argv);
	
	char saveName[256];
	//string ImgName;
	//char imgname[215];
	int count =0;
	long  totalframe = capture.get(CV_CAP_PROP_FRAME_COUNT);
        int  frametostart = 0;
        capture.set( CV_CAP_PROP_POS_FRAMES,frametostart);
        int fpsframe = capture.get(CV_CAP_PROP_FPS);
	int index = 0;
	while(1){
		
		//index = count++ * fpsframe+0; 
		index = count++ * 4+0;
		if(index >= totalframe){
			printf("end of thread 0;\n");
			break;
		}    
		capture.set(CV_CAP_PROP_POS_FRAMES,index);
		capture.read(frame);
                resize(frame,frame,Size(frame.cols/scale,frame.rows/scale));
                Mat img = frame.clone();

		vector<Rect> found;
		vector<Rect> found_filtered;
		// run the detector with default parameters. to get a higher hit-rate
		// (and more false alarms, respectively), decrease the hitThreshold and
		// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
		hog.detectMultiScale(img, found,0,Size(8,8),Size(32,32), 1.05,2);
		//int i, j;
		for( i = 0; i < found.size(); i++ ){
			Rect r = found[i];
			for( j = 0; j < found.size(); j++ )
				if( j != i && (r & found[j]) == r)
					break;
			if( j == found.size() )
				found_filtered.push_back(r);
		}//end for;

		for( i = 0; i < found_filtered.size(); i++ ){
			Rect r = found_filtered[i];
			// the HOG detector returns slightly larger rectangles than the real objects.
			// so we slightly shrink the rectangles to get a nicer output.
			if(gethardexaple){
				//////////////////////////////////////////////////////////////////////////////////
				r.x +=  cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				if(r.x <0 ||r.y <0 ||r.x+r.width >img.cols ||r.y + r.height >img.rows){
					continue;
				}
				else{
					Mat hardExampleImg = img(r);
					resize(hardExampleImg,hardExampleImg,Size(64,64));
					sprintf(saveName,"../result/result-hardexample6464/hard%d.jpg",hardExampleCount++);
					imwrite(saveName, hardExampleImg);
				}
				//////////////////////////////////////////////////////////////////////////////////
			}
			{
				r.x +=  cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				rectangle(frame, r.tl(), r.br(),Scalar(0,255,0), 3);
			}		
		}//end for  found-filtered;
		{
			//imshow("result",frame);
			//waitKey(100);
			sprintf(saveName,"../result/result-6464/image%d.jpg",index);
			imwrite(saveName, frame);
		}
		//read the next frame:
		//capture.set(CV_CAP_PROP_POS_FRAMES,count * fpsframe);
	}//end whilei
	return (void *)0;
}

void *frame2(void *arg){
	
	printf("1");
	char *argv;
	argv = (char *)arg;
	VideoCapture capture;//(argv[1]);
	Mat frame;
	capture.open(argv);


	char saveName[256];
	//string ImgName;
	//char imgname[215];
	int count =0;
	long  totalframe = capture.get(CV_CAP_PROP_FRAME_COUNT);
        int  frametostart = 0;
        capture.set( CV_CAP_PROP_POS_FRAMES,frametostart);
        int fpsframe = capture.get(CV_CAP_PROP_FPS);
	int index = 0;
	while(1){
		
		//index = count++ * fpsframe+1;
		 index = count++ * 4+1;
		if(index >= totalframe){
			printf("end of thread 0;\n");
			break;
		}    
		capture.set(CV_CAP_PROP_POS_FRAMES,index);
		capture.read(frame);
                resize(frame,frame,Size(frame.cols/scale,frame.rows/scale));
                Mat img = frame.clone();
		
		vector<Rect> found;
		vector<Rect> found_filtered;
		// run the detector with default parameters. to get a higher hit-rate
		// (and more false alarms, respectively), decrease the hitThreshold and
		// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
		hog.detectMultiScale(img, found,0,Size(8,8),Size(32,32), 1.05,2);
		//int i, j;
		for( i = 0; i < found.size(); i++ ){
			Rect r = found[i];
			for( j = 0; j < found.size(); j++ )
				if( j != i && (r & found[j]) == r)
					break;
			if( j == found.size() )
				found_filtered.push_back(r);
		}//end for;

		for( i = 0; i < found_filtered.size(); i++ ){
			Rect r = found_filtered[i];
			// the HOG detector returns slightly larger rectangles than the real objects.
			// so we slightly shrink the rectangles to get a nicer output.
			if(gethardexaple){
				//////////////////////////////////////////////////////////////////////////////////
				r.x +=  cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				if(r.x <0 ||r.y <0 ||r.x+r.width >img.cols ||r.y + r.height >img.rows){
					continue;
				}
				else{
					Mat hardExampleImg = img(r);
					resize(hardExampleImg,hardExampleImg,Size(64,64));
					sprintf(saveName,"../result/result-hardexample6464/hard%d.jpg",hardExampleCount++);
					imwrite(saveName, hardExampleImg);
				}
				//////////////////////////////////////////////////////////////////////////////////
			}
			{
				r.x +=  cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				rectangle(frame, r.tl(), r.br(),Scalar(0,255,0), 3);
			}		
		}//end for  found-filtered;
		{
			//imshow("result",frame);
			//waitKey(100);
			sprintf(saveName,"../result/result-6464/image%d.jpg",index);
			imwrite(saveName, frame);
		}
		//read the next frame:
		//capture.set(CV_CAP_PROP_POS_FRAMES,count * fpsframe);
	}//end whilei

	return (void *)0;
}

void *frame3(void *arg){
	printf("2");
	char *argv;
	argv = (char *)arg;
	VideoCapture capture;//(argv[1]);
	Mat frame;
	capture.open(argv);	


	char saveName[256];
	//string ImgName;
	//char imgname[215];
	int   count =0;
	long  totalframe = capture.get(CV_CAP_PROP_FRAME_COUNT);
        int  frametostart = 0;
        capture.set( CV_CAP_PROP_POS_FRAMES,frametostart);
        int fpsframe = capture.get(CV_CAP_PROP_FPS);
	int index = 0;
	while(1){
		
		//index = count++ * fpsframe+2; 
		index = count++*4+2;	
		if(index >= totalframe){
			printf("end of thread 0;\n");
			break;
		}    
		capture.set(CV_CAP_PROP_POS_FRAMES,index);
			
		capture.read(frame);
                resize(frame,frame,Size(frame.cols/scale,frame.rows/scale));
                Mat img = frame.clone();
		
		vector<Rect> found;
		vector<Rect> found_filtered;
		// run the detector with default parameters. to get a higher hit-rate
		// (and more false alarms, respectively), decrease the hitThreshold and
		// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
		hog.detectMultiScale(img, found,0,Size(8,8),Size(32,32), 1.05,2);
		//int i, j;
		for( i = 0; i < found.size(); i++ ){
			Rect r = found[i];
			for( j = 0; j < found.size(); j++ )
				if( j != i && (r & found[j]) == r)
					break;
			if( j == found.size() )
				found_filtered.push_back(r);
		}//end for;

		for( i = 0; i < found_filtered.size(); i++ ){
			Rect r = found_filtered[i];
			// the HOG detector returns slightly larger rectangles than the real objects.
			// so we slightly shrink the rectangles to get a nicer output.
			if(gethardexaple){
				//////////////////////////////////////////////////////////////////////////////////
				r.x +=  cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				if(r.x <0 ||r.y <0 ||r.x+r.width >img.cols ||r.y + r.height >img.rows){
					continue;
				}
				else{
					Mat hardExampleImg = img(r);
					resize(hardExampleImg,hardExampleImg,Size(64,64));
					sprintf(saveName,"../result/result-hardexample6464/hard%d.jpg",hardExampleCount++);
					imwrite(saveName, hardExampleImg);
				}
				//////////////////////////////////////////////////////////////////////////////////
			}
			{
				r.x +=  cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				rectangle(frame, r.tl(), r.br(),Scalar(0,255,0), 3);
			}		
		}//end for  found-filtered;
		{
			//imshow("result",frame);
			//waitKey(100);
			sprintf(saveName,"../result/result-6464/image%d.jpg",index);
			imwrite(saveName, frame);
		}
		//read the next frame:
		//capture.set(CV_CAP_PROP_POS_FRAMES,count * fpsframe);
	}//end whilei

	return (void *)0;
}

void *frame4(void *arg){
	printf("3");
	char *argv;
	argv = (char *)arg;
	VideoCapture capture;//(argv[1]);
	Mat frame;
	capture.open(argv);	


	char saveName[256];
	//string ImgName;
	//char imgname[215];
	int   count =0;
	long  totalframe = capture.get(CV_CAP_PROP_FRAME_COUNT);
        int  frametostart = 0;
        capture.set( CV_CAP_PROP_POS_FRAMES,frametostart);
        int fpsframe = capture.get(CV_CAP_PROP_FPS);
	int index = 0;
	while(1){
		
		//index = count++ * fpsframe+3; 
		index = count++*4+3;
		if(index >= totalframe){
			printf("end of thread 0;\n");
			break;
		}    
		capture.set(CV_CAP_PROP_POS_FRAMES,index);
		
		capture.read(frame);
                resize(frame,frame,Size(frame.cols/scale,frame.rows/scale));
                Mat img = frame.clone();
		
		vector<Rect> found;
		vector<Rect> found_filtered;
		// run the detector with default parameters. to get a higher hit-rate
		// (and more false alarms, respectively), decrease the hitThreshold and
		// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
		hog.detectMultiScale(img, found,0,Size(8,8),Size(32,32), 1.05,2);
		//int i, j;
		for( i = 0; i < found.size(); i++ ){
			Rect r = found[i];
			for( j = 0; j < found.size(); j++ )
				if( j != i && (r & found[j]) == r)
					break;
			if( j == found.size() )
				found_filtered.push_back(r);
		}//end for;

		for( i = 0; i < found_filtered.size(); i++ ){
			Rect r = found_filtered[i];
			// the HOG detector returns slightly larger rectangles than the real objects.
			// so we slightly shrink the rectangles to get a nicer output.
			if(gethardexaple){
				//////////////////////////////////////////////////////////////////////////////////
				r.x +=  cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				if(r.x <0 ||r.y <0 ||r.x+r.width >img.cols ||r.y + r.height >img.rows){
					continue;
				}
				else{
					Mat hardExampleImg = img(r);
					resize(hardExampleImg,hardExampleImg,Size(64,64));
					sprintf(saveName,"../result/result-hardexample6464/hard%d.jpg",hardExampleCount++);
					imwrite(saveName, hardExampleImg);
				}
				//////////////////////////////////////////////////////////////////////////////////
			}
			{
				r.x +=  cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				rectangle(frame, r.tl(), r.br(),Scalar(0,255,0), 3);
			}		
		}//end for  found-filtered;
		{
			//imshow("result",frame);
			//waitKey(100);
			sprintf(saveName,"../result/result-6464/image%d.jpg",index);
			imwrite(saveName, frame);
		}
		//read the next frame:
		//capture.set(CV_CAP_PROP_POS_FRAMES,count * fpsframe);
	}//end whilei

	return (void *)0;
}
