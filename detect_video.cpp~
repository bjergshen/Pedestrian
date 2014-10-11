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


double getTimeval(){
	struct rusage stRusage;
	struct timeval stTimeval;
//	gettimeofday(&stTimeval, NULL);
	getrusage(RUSAGE_SELF, &stRusage);
	stTimeval = stRusage.ru_utime; 
 
	return stTimeval.tv_sec + (double)stTimeval.tv_usec*1E-6; 
}

int hardExampleCount = 0; 
int gethardexaple = 0;
int scale = 1;
int main(int argc,char** argv )
{	
	char saveName[256];
	string ImgName;
	char imgname[215];
	FILE *fin_detector = fopen("../detector/default_detector6464.txt","r");
	float temp;
	char c;
	vector<float> myDetector;//3781 
	while(!feof(fin_detector))
	{
		fscanf(fin_detector,"%f",&temp);//fin_detector >> temp;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		myDetector.push_back(temp);
	}

	HOGDescriptor hog(cvSize(64,64),cvSize(16,16),cvSize(8,8),cvSize(8,8),9);  
	hog.setSVMDetector(myDetector);
	//hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());


	
	char str[200];
	Rect objpos;
	Point objcenter;
	vector<Point> objcenter_es;
	vector<Rect> objpos_es;
	vector<int> flag_ground;
	int objnum;
	int posx,posy;

	 VideoCapture capture(argv[1]);
         Mat frame;
        long  totalframe = capture.get(CV_CAP_PROP_FRAME_COUNT); 
        int  frametostart = 0;
	capture.set( CV_CAP_PROP_POS_FRAMES,frametostart);
	int fpsframe = capture.get(CV_CAP_PROP_FPS);
	int count = 0;

	//time counts start;
	double t_start = (double)getTimeval();
	double t_end;
	while(1)
	{
		capture.read(frame);
		//resize(frame,frame,Size(frame.cols/scale,frame.rows/scale));
		Mat img = frame.clone();
		
		vector<Rect> found;
		vector<Rect> found_filtered;
	        // run the detector with default parameters. to get a higher hit-rate
        	// (and more false alarms, respectively), decrease the hitThreshold and
	        // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
		hog.detectMultiScale(img, found,0,Size(8,8),Size(32,32), 1.05,2);
        	int i, j,k;
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
			sprintf(saveName,"../result/result-6464-0/image%d.jpg",hardExampleCount++);
			imwrite(saveName, frame);
		}
		//read the next frame:
		count++;
		if(count >=totalframe){
		//if(count *fpsframe >=totalframe){
			printf("end of the video!\n");
			break;
		}	
		//capture.set(CV_CAP_PROP_POS_FRAMES,count * fpsframe);
		capture.set(CV_CAP_PROP_POS_FRAMES,count);
	}//end while
			
	//time ends;
	t_end = (double)getTimeval();
	printf("time is    %10.70lf \n",(double)(t_end - t_start));	
	fclose(fin_detector);
	return 0;
}
