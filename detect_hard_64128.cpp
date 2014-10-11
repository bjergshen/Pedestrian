/*#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

using namespace cv;
using namespace std;

int hardExampleCount = 0; 
int gethardexaple = 1;
int scale = 1;
int main(int argc,char** argv )
{	
	Mat src;
	char saveName[256];
	string ImgName;
	char imgname[215];
	FILE *fin_detector = fopen("0915Body64128.txt","r");
	FILE *fin_imgList  =fopen("detectlist.txt","r");
	FILE *result = fopen("hardexample_list.txt","a+");  

	float temp;
	char c;
	vector<float> myDetector;//3781 
	while(!feof(fin_detector))
	{
		fscanf(fin_detector,"%f",&temp);//fin_detector >> temp;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		fscanf(fin_detector,"%c",&c);//fin_detector>>c;
		//cout<<temp<<"     "<<testdetectornum++<<endl;
		myDetector.push_back(temp);//放入检测器数组
	}

	HOGDescriptor hog(cvSize(64,128),cvSize(16,16),cvSize(8,8),cvSize(8,8),9);  
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
	while(!feof(fin_imgList))//while(getline(fin_imgList,ImgName))
	{
		fscanf(fin_imgList,"%s",imgname);
		printf("image %s \n",imgname);
		//string fullName = ImgName;
		src = imread(imgname);
//		imshow("test",src);
//		waitKey(0);
		resize(src,src,Size(src.cols/scale,src.rows/scale));
		Mat img = src.clone();
		
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

		int temperror = 0;
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
				if(r.width <64||r.height <128 ||r.x <0 ||r.y<0 ||r.x +r.width >src.cols ||r.y+r.height>src.rows )
					continue;
				else{
					
					Mat hardExampleImg = src(r);
					resize(hardExampleImg,hardExampleImg,Size(64,128));			
					sprintf(saveName,"result-hardexample/image%d.jpg",hardExampleCount++);
					fprintf(result,"%s\n",saveName);
					imwrite(saveName, hardExampleImg);
				}
			//////////////////////////////////////////////////////////////////////////////////
			}
			else{
				r.x +=  cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				rectangle(img, r.tl(), r.br(),Scalar(0,255,0), 3);
				cvRound
			}		
		}//end for  found-filtered;
		//imshow("result",img);
		//waitKey(100);
		//sprintf(saveName,"result-64128/image%d.jpg",hardExampleCount++);
		//imwrite(saveName, img);
	}//end while
	fclose(fin_detector);
	fclose(fin_imgList);
	fclose(result);
	return 0;
}
