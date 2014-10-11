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

    FILE* f = 0;//ÍŒÏñÎÄŒþÃû;

    FILE* finf =0;//ÍŒÏñµÄËµÃ÷ÐÅÏ¢ÎÄŒþÃû;

    char _filename[1024];

    char savename[512];



    if( argc == 1 )

    {

        printf("Usage: peopledetect (<image_filename> | <image_list>.txt)\n");

        return 0;

    }

    img = imread(argv[1]);

    

    if( img.data )//if the input is a image,processing directly;else it ishould be a file that contains the name of the images;

    {

        strcpy(_filename,argv[1]);

    }

    else//ÊäÈëµÄÊÇÍŒÏñÁÐ±í£¬ÔòÓŠžÃÍ¬Ê±¶ÁÈ¡ÍŒÏñÁÐ±í¶ÔÓŠµÄËµÃ÷ÐÅÏ¢ÎÄŒþ¡£

    {

        f = fopen("/home/td/code/facelist.txt", "rt");

        if(!f)

        {

            fprintf( stderr, "ERROR: the specified file could not be loaded\n");

            return -1;

        }

    }



	int i=0;

	while(1)

    {



        char* filename = _filename;



		///////////////////ÍŒÏñÎÄŒþ¶ÁÈ¡;

       if(!fgets(filename, (int)sizeof(_filename)-2, f))//»ñÈ¡ÎÄŒþÃûÊ§°Ü£¿£¿£¿-2µÄÔ­ÒòÔÚÓÚ×îºóÁœžöÊÇ"\n" "\0".ÎÄŒþµÄ¶ÁÈ¡»áÔÚÓöµœ"\n"Ê±Í£Ö¹¡£ŒŽÖ»¶ÁÈ¡Ò»ÐÐ¡£

                break;

        //while(*filename && isspace(*filename))

        //  ++filename;

        if(filename[0] == '#')

            continue;

        int l = (int)strlen(filename);

        while(l > 0 && isspace(filename[l-1]))//ÎÄŒþÃûŽóÓÚ0³€¶È£¬ºÏ·š¡£×Œ±ž¶ÁÈ¡ÏÂÒ»žöÎÄŒþÃû¡£

            --l;

        filename[l] = '\0';//Ö®Ç°µÄl-1ÊÇ×Ö·ûŽ®µÄÏÂ±êÊÇÊµŒÊÎ»ÖÃ-1£¬×îºóµÄÎ»ÖÃÒªŒÓÉÏœáÎ²±êÖŸ£º"\0"£»

	sprintf(savename, "%s%s","/home/td/code/",filename);	
	img = imread(savename);
	imshow("test",img);
	waitKey(10);
		printf("%d \n",i+1);

	  printf("%s:\n", savename);
	   if(!img.data)//ÍŒÏñ¶ÁÈ¡ŽíÎó;

		   break;

	   Mat dstimg;



	   resize(img,dstimg,Size(64,128));

		//Mat dstimg = img(Rect(0,0,64,128));		

		sprintf(savename, "%s%d%s","/home/td/code/facenormal",++i, ".jpg");//±£ŽæµÄÍŒÆ¬Ãû

	    //sprintf(savename, "del %s",_filename);//±£ŽæµÄÍŒÆ¬Ãû

		//system(savename);

		String  resultname = savename;

		imwrite(resultname,dstimg);

	}

	return 0;

} 
