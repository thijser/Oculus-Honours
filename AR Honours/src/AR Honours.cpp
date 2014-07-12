//============================================================================
// Name        : AR.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include <cv.h>
#include <highgui.h>
using namespace cv;

using namespace cv;


Mat bleur(Mat input){
	Mat ret;

	Mat element = getStructuringElement( 2,
	                                       Size( 2*4 + 1, 2*4+1 ),
	                                       Point( 4, 4 ) );

	erode(input, ret,element);
     return ret;
}
//sketch filter
Mat scetch(Mat input){
	Mat ret;
     cvtColor(input, ret, CV_BGR2GRAY);
     GaussianBlur(ret, ret, Size(7,7), 3, 3);
     Canny(ret, ret, 0, 32, 3);
     bitwise_not(ret,ret);
return ret;

}
//applies filters
Mat filter(Mat input){
	Mat out=scetch(input);
	return out;
}

void output(Mat left,Mat right){
	Mat imgResult(left.rows,2*left.cols,left.type());

	Mat roiImgResult_Left = imgResult(Rect(0,0,left.cols,left.rows));
	Mat roiImgResult_Right = imgResult(Rect(left.cols,0,right.cols,right.rows));

	Mat roiImg1 = left(Rect(0,0,left.cols,left.rows));
	Mat roiImg2 = right(Rect(0,0,right.cols,right.rows));

	roiImg1.copyTo(roiImgResult_Left); //Img1 will be on the left of imgResult
	roiImg2.copyTo(roiImgResult_Right); //Img2 will be on the right of imgResult
	imshow("Finalimg",imgResult);


}
int main(int, char**)
{
    VideoCapture cap(0); //open cam
    if(!cap.isOpened())
        return -1;

    Mat edges;
    namedWindow("edges",1);
    while(true)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        Mat result=filter(frame);
        output(result,result);

        if(waitKey(1) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
