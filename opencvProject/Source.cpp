// trzeba wybra� odpowiednie pliki nag��wkowe w zale�no�ci od wersji
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>


#include <iostream>

int main( int argc, char** argv )
{
 cv::namedWindow("Okienko Prawie Inteligentne", 1);
 cv::waitKey(0);
 return 0;
}