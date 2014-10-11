#include "cv.h"  

#include "highgui.h"  

#include "math.h"

#include <ml.h>  

#include <iostream>  

#include <fstream>  

#include <string>  

#include <vector>  



using namespace cv;  

using namespace std;  

  

class Mysvm: public CvSVM  

{  

public:  

    int get_alpha_count()  

    {  

        return this->sv_total;  

    }  

  

    int get_sv_dim()  

    {  

        return this->var_all;  

    }  

  

    int get_sv_count()  

    {  

        return this->decision_func->sv_count;  

    }  

  

    double* get_alpha()  

    {  

        return this->decision_func->alpha;  

    }  

  

    float** get_sv()  

    {  

        return this->sv;  

    }  

  

    float get_rho()  

    {  

        return this->decision_func->rho;  

    }  

};  

int char2int(char *input);

int pow(int a,int b);

int help();

int main(int argc, char** argv)    

{    

	

	vector<string> img_path; 

	vector<int> img_catg;   

	int nLine = 0;           

 	int numPos;

	if(argc < 3){

		help();	

		return 0;

	}

	numPos = char2int(argv[2]);

	cout<<"train pos num is "<<numPos<<endl;  

	int featurenum = 3780;//1764 for 64*64    3780 for 64 *128

	int INRIAdata = 1;

	string buf;  

        ifstream svm_data( argv[1] ); 

   	unsigned long n;  

	while( svm_data )//read the input images;

    	{  

        	if( getline( svm_data, buf ) )  

        	{  

            		nLine ++; 

			img_path.push_back( buf );

        		if( nLine<=numPos ){  

                	 	img_catg.push_back(1);

		         }  

		         else{  

	               		img_catg.push_back(-1);  

        		 }  

        	}  

	}  

    svm_data.close();

    CvMat *data_mat, *res_mat;  

    int nImgNum = nLine;	

    data_mat = cvCreateMat( nImgNum, featurenum, CV_32FC1 );  

    cvSetZero( data_mat );  

     

    res_mat = cvCreateMat( nImgNum, 1, CV_32FC1 );  

    cvSetZero( res_mat );  



    /////IplImage* src;  

    ////IplImage* trainImg=cvCreateImage(cvSize(64,128),8,3);

	//CvRect roiRect;

	//roiRect.x=0;

	//roiRect.y=0;

	//roiRect.width=64;

	//roiRect.height=64;

    int tempcount=0;

    int cateindex = 0;

    for( string::size_type i =0; i != img_path.size(); i++ ){  

	++cateindex;

            /////src=cvLoadImage(img_path[i].c_str(),1);

//			cout<<"picture:  "<<" "<<i<<"index:  "<<img_catg[i]<<endl;

	Mat trainImg; 

			/*

			IplImage * tempimg;

			IplImage * tempdstimg;

			tempimg = cvLoadImage(img_path[i].c_str(),1);

			if(cateindex<=numPos&&tempimg->nChannels ==1)

			{

				tempdstimg=cvCreateImage(cvSize(128,64),8,3); 

				cvMerge(tempimg,tempimg,tempimg,0,tempdstimg);

				trainImg = tempdstimg;

			}

			else

			*/

			//tempimg = cvLoadImage(img_path[i].c_str(),1);

	trainImg = cvLoadImage(img_path[i].c_str(),1);

	if(0 == i%100){

		imshow("test",trainImg);

		cvWaitKey(100);

	}	

	if( trainImg.data == NULL ){  

	        cout<<" can not load the image: "<<img_path[i].c_str()<<endl;  

                continue;  

        }  

        //// cout<<" processing "<<img_path[i].c_str()<<endl;              

        //cvResize(src,trainImg);   //读取涂片     

        HOGDescriptor hog(cvSize(64,128),cvSize(16,16),cvSize(8,8),cvSize(8,8),9);  

        vector<float> descriptors;

			//if(i>=numPos)

			//{

			//	//IplImage* srcneg=0;//客是trainImg;

			//	IplImage* dstneg=0;

			//	CvSize dstneg_size;

			//	dstneg_size.width  = 64;

			//	dstneg_size.height = 128;

			//	CvRect roiRect;

			//	roiRect.x=(trainImg->width)/2-48;

			//	roiRect.y=(trainImg->height)/2-80;

			//	roiRect.width=64;

			//	roiRect.height=128;

			//	//dstneg= cvCreateImage(dstneg_size,trainImg->depth,trainImg->nChannels);

			//	//cvResize(trainImg,dstneg,CV_INTER_LINEAR);

			//	cvSetImageROI(trainImg, roiRect);

			//	dstneg = cvCreateImage(dstneg_size,trainImg->depth,trainImg->nChannels);

			//	cvCopy(trainImg, dstneg, NULL);

			//	cvResetImageROI(trainImg);

			//	hog->compute(dstneg, descriptors,Size(1,1), Size(0,0)); //调用科算函数开士科算，酷果挎竣在descriptor;

			//	cvReleaseImage(&dstneg);

			//}

			//else

			/*

			tempcount++;

			Rect region_of_interest;

			Mat dstimg;

			if(tempcount<=numPos)

			{

				region_of_interest = Rect(16,16,64,64);

				dstimg = trainImg(region_of_interest);

			}

			else

			{

					region_of_interest = Rect(0,0,64,64);

					dstimg = trainImg(region_of_interest);

			}

			*/

			//Mat trainImg ;

			//if(cateindex <= numPos &&INRIAdata == 1)

			//	trainImg = tempimg(Rect(16,16,64,128));

			//else

			//	trainImg =  tempimg(Rect(0,0,64,128));

	hog.compute(trainImg, descriptors,Size(8,8)); 

        n=0;  

        for(vector<float>::iterator iter=descriptors.begin();iter!=descriptors.end();iter++){

	//vector<float>::iterator iter=descriptors.begin();

        //for(int k=0;k<1764;k++)

		cvmSet(data_mat,i,n,*iter);//把HOG挎竣下揽  

	        n++;  

        }  

            //cout<<SVMtrainMat->rows<<endl;  

	cvmSet( res_mat, i, 0, img_catg[i] ); 

        ///cout<<" end processing "<<img_path[i].c_str()<<" "<<img_catg[i]<<endl;  

    }  

	//////////////////////////////////////////////////////////////debug

	//cout<<"Saving the descriptors Mat"<<endl;



	//cvSave("mat.yml",data_mat);

 //   cout<<"Success Saving the descriptors Mat"<<endl;



	Mysvm svm;//= CvSVM();//新靠一况SVM    

   	CvSVMParams params;    

	params.svm_type = CvSVM::C_SVC;    

	params.kernel_type = CvSVM::LINEAR;    

	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, FLT_EPSILON);  

	params.C = 0.01; 

	CvTermCriteria criteria;    

	criteria = cvTermCriteria( CV_TERMCRIT_EPS, 1000, FLT_EPSILON );    

   	cout<<"Trainning the SVM: "<<endl;  

   	svm.train( data_mat, res_mat, NULL, NULL, params );     

	//////////////////////////////////////////////////////////////debug

	//svm.load("SVM_DATA.xml");

	//svm.save( "SVM_DATA.xml" );

	//cout<<"SAVED SVM DATA: "<<endl; 

	

	cvReleaseMat( &data_mat );  

	cvReleaseMat( &res_mat );  





	

	cout<<"Getting the weight: "<<endl; 

	



	

	int supportVectorSize =svm.get_support_vector_count();  

	CvMat *sv,*alp,*re;

	sv  = cvCreateMat(supportVectorSize , featurenum, CV_32FC1);  

	alp = cvCreateMat(1 , supportVectorSize, CV_32FC1);  

	re  = cvCreateMat(1 , featurenum, CV_32FC1);  

	CvMat *res  = cvCreateMat(1 , 1, CV_32FC1);  

	cvSetZero(sv);  

	cvSetZero(re);  

	cvSetZero(alp);

    

	for(int i=0; i<supportVectorSize; i++){  

		memcpy( (float*)(sv->data.fl+i*featurenum), svm.get_support_vector(i), featurenum*sizeof(float));      

	}  

	double* alphaArr = svm.get_alpha(); 

	int alphaCount = svm.get_sv_count();  



	for(int i=0; i<supportVectorSize; i++){  

		alp->data.fl[i] = alphaArr[i];  

	}  

	cvMatMul(alp, sv, re);  



	int posCount = 0;  

	for (int i=0; i<featurenum; i++)  

	{  

		re->data.fl[i] *= -1;  

	}  

	FILE* fp;

	if(argc <4){

		fp = fopen("../default_detector64128.txt","wb");	

	}

	else

		fp = fopen(argv[3],"wb");  

	if( NULL == fp )  

	{  

		cout<<"fail to write the file :hogSVMDetector-peopleFlow.txt;"<<endl; 

		return 1;  

	}  

	for(int i=0; i<featurenum; i++)  

	{  

		//fprintf(fp,"%f  %c",re->data.fl[i],',');  

		fprintf(fp,"%f  ",re->data.fl[i]);  

	}  

	float rho = svm.get_rho();

	fprintf(fp, "%f", rho);  

	cout<<"hogSVMDetector.txt saved!"<<endl;

	fclose(fp); 

	cout<<"Success getting the weight "<<endl; 



	cvReleaseMat( &sv); 

	cvReleaseMat( &alp); 

	cvReleaseMat( &re); 





	svm.save( "SVM_DATA.xml" );

	/////////////////////////////////////////////// 

	cvWaitKey(0);

	getchar();

	return 0;  

}

int char2int(char* input)

{

//        char input[] = "123";

        int length = strlen(input);

        int output = 0;

        char c;

        int i;

        printf("%d\n",length);

        for(i=length-1;i>=0;i--){

                 output += pow(10,i)*(input[length - i -1]-'0');

        }

        printf("%d\n",output);

        return output;

}

int pow(int a,int b){

        int i;

        int output = 1;

        for(i = 0;i<b;i++){

                output *= a;

        }

        return output;

}

int help(){

	cout<<"the app is used to train the svm detector for 64*64 obj"<<endl;

	cout<<"the input para should contain at least 3 paras : "<<endl;

	cout<<"0: execute   1: train data-list.txt  2: the pos data-num (int)  3:the output detector filename.txt (or default)"<<endl;

	return 0;

}
