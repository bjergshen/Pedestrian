#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

using namespace cv;
using namespace std;

int hardExampleCount = 0; 
int gethardexample = 0;
int scale = 1;
double thresh = 0;
int sizetype = 64;
int help();

int main(int argc,char** argv )
{	
	Mat src;
	char saveName[256];
	string ImgName;
	char imgname[215];
	FILE *fin_detector;
	FILE *fin_imgList;
	FILE *fout_resultlist; 
	printf("the argc num is %d\n\n",argc);
	//////0:execute 1:64 or 128 2:gethardexample 3:detectthresh 4:detectorname(txt)5:imagelist(txt)  6:detect-result(txt)
	if(argc <7){
		fout_resultlist = fopen("../result/defaule_resultpath.txt","wb");
	}
	else{
		fout_resultlist = fopen(argv[6],"wb");
		puts("resutl_list is :");
		puts(argv[6]);
	}
	if(argc <6){
		//help();
		fin_imgList = fopen("../datasource/default_obj_list.txt","r");
	}
	else{
		fin_imgList  =fopen(argv[5],"r");
		puts("img_list is :");
		puts(argv[5]);
	}
	if(argc <5){
		//help();
		if(argc <2){
			fin_detector = fopen("../detector/default_detector6464.txt","r");
		}
		else
			fin_detector = fopen("../detector/default_detector64128.txt","r");
	}
	else{
		fin_detector = fopen(argv[4],"r");
		puts("detector is :");
		puts(argv[4]);
	}
	if(argc <4){
		//help()
		//default 0;
	}
	else{
		//thresh = char2double(argv[3]);
		thresh = atof(argv[3]);
		puts(argv[3]);
		printf("thresh is %lf \n",thresh);
	}
	if(argc <3){
		//help();
		//default 0;
	}
	else{
		if('0'==argv[2][0]){
			gethardexample = 0;
		}
		else
			gethardexample = 1;
		printf("gethardexample:%d \n",gethardexample);

	}
	if(argc <2){
		help();
		return -1;	
	}
	else{
		//if(64 ==char2double(argv[1]))
		if(64 == atof(argv[1]))
			sizetype = 64;
		else
			//if(128 == char2double(argv[1]))
			if(128 == atof(argv[1]))
				sizetype = 128;
			else{
				printf("input para is not the correct type ,see help ,the para will be default :64");
			}
		printf("sizetype :%d \n",sizetype);
	}

	float temp;
	char c;
	vector<float> myDetector;//3781 
	while(!feof(fin_detector))
	{
		fscanf(fin_detector,"%f",&temp);//fin_detector >> temp;
		myDetector.push_back(temp);
		printf("%f   ",temp);
	}
	waitKey(10000);
	//printf("size of detector is :%ld \n",myDetector.size());

	HOGDescriptor hog(cvSize(64,sizetype),cvSize(16,16),cvSize(8,8),cvSize(8,8),9);  
	hog.setSVMDetector(myDetector);
	//hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	char str[128];
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
		if(!src.data){
			printf("fail to load image :%s \n",imgname);
		}
		//		imshow("test",src);
		//		waitKey(0);
		resize(src,src,Size(src.cols/scale,src.rows/scale));
		Mat img = src.clone();//new a space for img;differ to  img(src)

		vector<Rect> found;
		vector<Rect> found_filtered;
		// run the detector with default parameters. to get a higher hit-rate
		// (and more false alarms, respectively), decrease the hitThreshold and
		// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
		double t = (double)getTickCount();
		hog.detectMultiScale(img, found,thresh,Size(8,8),Size(32,32), 1.05,2);
		t = (double)getTickCount() - t;
		printf("tdetection time = %gms\n", t*1000./cv::getTickFrequency());
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
			if(gethardexample){
				//////////////////////////////////////////////////////////////////////////////////
				r.x +=  cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				if(r.width <64||r.height <64 ||r.x <0 ||r.y<0 ||r.x +r.width >src.cols ||r.y+r.height>src.rows )
					continue;
				else{

					Mat hardExampleImg = src(r);//share the same space;
					resize(hardExampleImg,hardExampleImg,Size(64,sizetype));			
					if(64 == sizetype){
						sprintf(saveName,"../result/result-hardexample6464/image%d.jpg",hardExampleCount++);
					}
					else{
						sprintf(saveName,"../result/result-hardexample64128/image%d.jpg",hardExampleCount++);
					}
					fprintf(fout_resultlist,"%s\n",saveName);
					imwrite(saveName, hardExampleImg);
					//hardExampleImg.release();
				}
				//////////////////////////////////////////////////////////////////////////////////
			}
			r.x +=  cvRound(r.width*0.1);
			r.width = cvRound(r.width*0.8);
			r.y += cvRound(r.height*0.07);
			r.height = cvRound(r.height*0.8);
			rectangle(img, r.tl(), r.br(),Scalar(0,255,0), 3);
		}//end for  found-filtered;
		if(gethardexample){
			if(64 == sizetype){
				sprintf(saveName,"../result/result-6464/image%d.jpg",hardExampleCount++);
			}
			else{
				sprintf(saveName,"../result/result-64128/image%d.jpg",hardExampleCount++);
			}
			//fprintf(fout_resultlist,"%s",saveName);
			imwrite(saveName, img);
		
		}
		else{
			imshow("result",img);
			waitKey(10);
		}
	}//end while
	fclose(fin_detector);
	fclose(fin_imgList);
	fclose(fout_resultlist);
	return 0;
}

int help(){

	puts("the app is used to detect obj by  the svm detector trained with size  \n\n");

	puts("  0: execute  \n  1:64 or 128   \n  2:gethardexample  \n  3: detect-thresh  \n  ");

	puts("  4: detector.txt default:default_detector6464.txt   \n  5: obj-to-detect-list.txt(the image list)default:default_obj_list.txt   \n  6:the output of detected objs path.txt (or default)\n  ");
	//0:execute 1:64 or 128 2:gethardexample 3:detectthresh 4:detectorname(txt)5:imagelist(txt)  6:detect-result(txt)
	return 0;

}
