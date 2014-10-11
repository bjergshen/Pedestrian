#include "cv.h"  
#include "highgui.h"  
#include <ml.h>  
#include <time.h>
#include <iostream>  
#include <fstream>  
#include <string>  
#include <vector>  

using namespace cv;  
using namespace std;  

int main(int argc,char** argv)
{
	Mat img;
	Mat flipimg;
	FILE* f = 0;
	FILE* finf =0;
	char _filename[1024];
	char savename[512];
	if( argc < 3 ){
		printf("Usage:   <image_list>.txt   <image_result_save_path>)\n");
		return 0;
	}
	strcpy(_filename,argv[1]);
	f = fopen(_filename, "rt");
	if(!f){
		fprintf( stderr, "ERROR: the specified file could not be loaded\n");
		return -1;
	}
	int i=0;
	while(1){
		char* filename = _filename;
		if(!fgets(filename, (int)sizeof(_filename)-2, f))
			break;
		if(filename[0] == '#')
			continue;
		int l = (int)strlen(filename);
		while(l > 0 && isspace(filename[l-1]))
			--l;
		filename[l] = '\0';
		img = imread(filename);
		printf("%d \n",i+1);
		if(!img.data)
			break;
		flip(img,flipimg,1);//para 3 :0 up-down;  >0 left-right; 
		//Mat dstimg;
		//resize(img,dstimg,Size(64,128));
		//Mat dstimg = img(Rect(0,0,64,128));		
		sprintf(savename, "%simg%d%s",argv[2],++i, ".jpg");
		String  resultname = savename;
		imwrite(resultname,flipimg);
	}//end while 
	return 0;
} 
