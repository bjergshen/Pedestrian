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

double char2double(char *input);
double char2demi(char *input);
double char2int(char *input);
double mypow(double a,int b);
int help();

int main(int argc, char** argv)    

{    
	int sizetype;//64*64 or 64 *128
	int featurenum = 1764;//1764 for 64*64    3780 for 64 *128
	char name[128];
	vector<string> img_path; 
	vector<int> img_catg;   
	ifstream img_list;
	FILE* detector;

	int nLine = 0;           
	int numPos;
	////0: execute  \n  1: positive image number   \n  2:height 64 or 128  default: 64\n  3:train-image-list  default :train_list\n  4:detector.txt default:default_detector64.txt   \n
	if(argc < 2){
		help();	
		return 0;

	}
	if(argc >=2){
		numPos = char2int(argv[1]);
		cout<<"num of pos :"<<numPos<<endl;
	}
	if(argc >=3){
		sizetype = char2int(argv[2]);
		cout<<"sizetype : "<<sizetype<<endl;
		if(64 == sizetype){
			featurenum = 1764;
		}
		else{
			featurenum = 3780;
		}
	}
	if(argc >= 4){
		img_list.open(argv[3]);
		cout<<"img_list "<<argv[3]<<endl;
	}
	else{
		img_list.open("../datasource/train_list.txt");
	}
	if(argc >= 5){
		detector = fopen(argv[4],"wb");
		if(!detector){
			cout<<"fail to create file :detector.txt"<<endl;
		}
		cout<<"detector "<<argv[4]<<endl;
	}
	else{
		detector = fopen("../detector/default_detector64.txt","wb");
	}

	string buf;  
	//ifstream img_list( argv[1] ); 
	unsigned long n;  
	//read the input images;
	while( img_list){  
		if( getline( img_list, buf ) ){
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
	img_list.close();
	
	CvMat *data_mat, *res_mat;  
	int nImgNum = nLine;	
	data_mat = cvCreateMat( nImgNum, featurenum, CV_32FC1 );  
	cvSetZero( data_mat );    
	res_mat = cvCreateMat( nImgNum, 1, CV_32FC1 );  
	cvSetZero( res_mat ); 

	int tempcount=0;
	int cateindex = 0;
	for( string::size_type i =0; i != img_path.size(); i++ ){  
		++cateindex;
		/////src=cvLoadImage(img_path[i].c_str(),1);
		//			cout<<"picture:  "<<" "<<i<<"index:  "<<img_catg[i]<<endl;
		Mat trainImg; 
		//tempimg = cvLoadImage(img_path[i].c_str(),1);
		trainImg = cvLoadImage(img_path[i].c_str(),1);
		if(0 == i%100){
			imshow("test",trainImg);
			cvWaitKey(100);
			//cvDestroyAllWindows();
		}	
		if( trainImg.data == NULL ){  
			cout<<" can not load the image: "<<img_path[i].c_str()<<endl;  
			continue;  
		}  
		//// cout<<" processing "<<img_path[i].c_str()<<endl;              
		//cvResize(src,trainImg);   //读取涂片     
		HOGDescriptor hog(cvSize(64,sizetype),cvSize(16,16),cvSize(8,8),cvSize(8,8),9);  
		vector<float> descriptors;

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
	destroyWindow("test");  
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
	//svm.load("img_list.xml");
	//svm.save( "img_list.xml" );
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
	for(int i=0; i<featurenum; i++)  
	{  
		//fprintf(detector,"%f  %c",re->data.fl[i],',');  
		fprintf(detector,"%f  ",re->data.fl[i]);  
	}  
	float rho = svm.get_rho();
	fprintf(detector, "%f", rho);  
	cout<<"hogSVMDetector.txt saved!"<<endl;
	fclose(detector); 
	cout<<"Success getting the weight "<<endl; 

	cvReleaseMat( &sv); 
	cvReleaseMat( &alp); 
	cvReleaseMat( &re); 

	svm.save( "../datasource/img_list.xml" );
	/////////////////////////////////////////////// 
	cvWaitKey(0);
	getchar();
	return 0;  
}

double char2double(char* input){

	//char input[] = "123";
	char font[256],back[256];
	int length = strlen(input);
	int index = 0;
	int i,j;
	double output = 0;
	for(index = 0;index <length;index++){
		if(input[index] == '.'){
			//strncpy(font,input,index);
			//strncpy(back,input,length-1-index);
			for(i = 0;i<index;i++){
				font[i] = input[i];
			}
			font[i] = '\0';
			for(i = index+1,j =0;i<length;i++,j++){
				back[j] = input[i];
			}
			back[j] = '\0';
			output += char2int(font);
			//printf("+int: %f \n",output);
			output += char2demi(back);
		}				
	}
//	printf("output %.12f \n",output);
	int  temp = output*100000;
//	printf("int temp%10d \n",temp);
	return temp/100000.0;
}
double char2int(char *input){
	int length = strlen(input);
	double output = 0;
	char c;
	int i;
	//printf("%s\n",input);
	for(i=length-1;i>=0;i--){
		output += mypow(10,i)*(input[length - i -1]-'0');
	}
//	printf("char2int  %.10f\n",output);
	return output;
}
double char2demi(char *input){
	int length = strlen(input);
	double output = 0;
	char c;
	int i;
//	printf("demi  length :%10d \n",length);
	for(i =0;i<length;i++){
		output += mypow(0.1,i+1)*(input[i]-'0');
//		printf("char2deimi %d  %.16f\n",i+1,output);
	}
//	printf("char2deimi  %.16f\n",output);
	return output;
}
double mypow(double a,int b){
	int i;
	double output = 1;
	for(i = 0;i<b;i++){
		output *= a;
//		printf("mypow : %d  %.16f\n",i+1,output);
	}
	return output;
}

int help(){

	puts("the app is used to detect obj by  the svm detector trained with size  \n\n");

	puts("  0: execute  \n  1: positive image number   \n  2:height 64 or 128  default: 64\n  3:train-image-list  default :train_list\n  4:detector.txt default:default_detector64.txt   \n");
	return 0;

}
