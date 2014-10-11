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

    FILE* f = 0;
    FILE* fout =0;
    char _filename[1024];
    char savename[512];

    f = fopen(argv[1], "rt");
    fout = fopen(argv[2],"wt");
    if(!f)

    {

         fprintf( stderr, "ERROR: the specified file could not be loaded\n");

         return -1;

     }

    int i=0;
    while(1)
    {
       char* filename;
       if(!fgets(filename,512, f))
                break;

        //while(*filename && isspace(*filename))

        //  ++filename;
        int l = (int)strlen(filename);

        while(l > 0 && isspace(filename[l-1]))

            --l;
        filename[l] = '\0';
        printf("%s:\n", filename);
	sprintf(savename, "%s%s","/home/td/code/crop/",filename);  
 	Mat img,dstimg;
      img = imread(savename);
      resize(img,dstimg,Size(64,128));
     // Mat dstimg = img(Rect(0,0,64,128));		
      sprintf(savename, "%s%d%s","/home/td/code/nomalface/face%d",++i, ".jpg");
      fprintf(fout,"%s",savename);
      fprintf(fout,"%c",'\n');
      //sprintf(savename, "del %s",_filename);//±£ŽæµÄÍŒÆ¬Ãû
      //system(savename);
      String  resultname = savename;
      imwrite(resultname,dstimg);

   }//end of while;

	return 0;

} 
