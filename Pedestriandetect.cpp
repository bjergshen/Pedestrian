 #include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"

 #include <iostream>
 #include <stdio.h>

 using namespace std;
 using namespace cv;




 /** 全局变量 */
 string pedestrian_cascade_name = "hogcascade_pedestrians.xml";
 //string eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
 CascadeClassifier face_cascade;
 CascadeClassifier eyes_cascade;
 string window_name = "Capture - Face detection";
 RNG rng(12345);
char saveName[512];


 /** 函数声明 */
void detectAndDisplay( Mat frame );

 /** @主函数 */
 int main( int argc, const char** argv )
 {
   CvCapture* capture;
   Mat frame;
   int startframe =1;

 
   
   //-- 1. 加载级联分类器文件
   if( !pedestrian_cascade.load( pedestrian_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
   //if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

   //-- 2. 打开内置摄像头视频流
   //capture = cvCaptureFromCAM( -1 );
   //capture = cvCaptureFromCAM(CV_CAP_ANY );
   
   	capture = cvCaptureFromAVI(argv[1]);

	//获取视频信息
	cvQueryFrame(capture);
	int frameH    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	int frameW    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	int fps       = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	int numFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

   if( capture )
   {

     while( true )
     {
		 cout<<startframe * fps<<"  /  "<<numFrames<<endl;
		 if((startframe++) * fps >=  numFrames)
		 {
			 cout<<"end of the file,the exe will exit !"<<endl;
		 }
		 cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES,(startframe++) * fps);
		 frame = cvQueryFrame( capture );

   //-- 3. 对当前帧使用分类器进行检测
       if( !frame.empty() )
       { detectAndDisplay( frame ); }
	 else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }
   return 0;
 }

/** @函数 detectAndDisplay */
void detectAndDisplay( Mat frame )
{
  static int count =0;
  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- 多尺寸检测人脸
  face_cascade.detectMultiScale( frame_gray, faces, 1.5, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( int i = 0; i < faces.size(); i++ )
  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
   // ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
	Rect facesave;
	facesave.x = (faces[i].x - faces[i].width /2.0)>0?(faces[i].x - faces[i].width /2.0):0;
	facesave.y = (faces[i].y - faces[i].height /2.0) > 0?(faces[i].y - faces[i].height /2.0):0;
	facesave.width = faces[i].width * 2;
	facesave.height = faces[i].width * 2;//为了方便后面的训练时样本的长宽一致.
	if((facesave.height + facesave.y > frame.rows)||(facesave.width + facesave.x > frame.cols))//预截取的部分超过了图像的边缘，放弃。
	{
		continue;
	}
	//facesave.height = faces[i].height * 2;
	//rectangle(frame,facesave.tl(),facesave.br(),Scalar(0,255,0),3);
	Mat face(frame,facesave);
	
	//string tempname = saceName;
	sprintf(saveName,"crop/face%d.jpg",count++);
	imwrite(saveName,face);
	face.release();
   /*
	Mat faceROI = frame_gray( faces[i] );
   
   std::vector<Rect> eyes;
    //-- 在每张人脸上检测双眼
    
	eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( int j = 0; j < eyes.size(); j++ )
     {
       Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       int radius = cvRound( (eyes[j].width + eyes[i].height)*0.25 );
       circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
     }
	 */
  }
  //-- 显示结果图像
  //imshow( window_name, frame );
 }
