// trzeba wybraæ odpowiednie pliki nag³ówkowe w zale¿noœci od wersji
#include <opencv2\opencv.hpp>

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <time.h>
#include <allheaders.h>
#include <iostream>
#include <conio.h>
using namespace cv;
using namespace std;

void mouseCallback(int event, int x, int y, int flags, void* userdata){
	if(event == EVENT_LBUTTONDBLCLK){
		cout<< x <<y<<endl;
	}
}
int main(int, char**)
{
    
	Mat frame;
	VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
	
	int iLowH = 170;
	int iHighH = 179;

	int iLowS = 150; 
	int iHighS = 255;

	int iLowV = 60;
	int iHighV = 255;
	
	int iLastX = -1; 
	int iLastY = -1;
	Mat imgTemp;
	cap.read(imgTemp);

	Mat imgLines = Mat::zeros(imgTemp.size(),CV_8UC3);;
	
	while (waitKey(30) !=27){
		
		Mat frame2;
		cap>>frame;
		cvtColor(frame,frame2,COLOR_BGR2HSV);
		/*cvtColor(frame,frame2, CV_BGR2GRAY);
		GaussianBlur(frame2, frame2, Size(7,7), 1.5, 1.5);;
		Canny(frame2,frame2, 0,30,3);*/
		
		Mat imgTres;
		inRange(frame2,Scalar(iLowH,iLowS,iLowV),Scalar(iHighH,iHighS,iHighV),imgTres);
		
		//morphological opening (removes small objects from the foreground)
		erode(imgTres, imgTres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		dilate( imgTres, imgTres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

   //morphological closing (removes small holes from the foreground)
		dilate( imgTres, imgTres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		erode(imgTres, imgTres, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

   //Calculate the moments of the thresholded image
		Moments oMoments = moments(imgTres);
		
		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

   // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
  if (dArea > 10000)
  {
   //calculate the position of the ball
   int posX = dM10 / dArea;
   int posY = dM01 / dArea;        
        
   if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
   {
    //Draw a red line from the previous point to the current point
    line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
   }
   
   iLastX = posX;
   iLastY = posY;
  }
		//setMouseCallback("camera",mouseCallback,NULL);
  
  imshow("przerobione",imgTres);
  frame2 = frame2 + imgLines; 
	imshow("camera",frame2);
	}
	
    
}
