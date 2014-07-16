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
#include <math.h>
#include <imgproc/imgproc.hpp>

using namespace cv;

using namespace cv;
std::vector<int> ra;

Mat fish ;
Mat base ;
bool f = false;
bool b = false;
int screenx=20;
int screeny=20;
int spacing=50;
Size imagesize(1280,960);

Mat genfish(Mat base) {
	std::vector<float> vec;
	double outer = -0.007715; // affects only the outermost pixels of the image
	double inner = 0.026731; // most cases only require b optimization
	double uniform = 0.0; // most uniform correction
	double scale = 1.0 - outer - inner - uniform; // describes the linear scaling of the image

	for (int x = 0; x < base.cols; x++) {
		for (int y = 0; y < base.rows; y++) {
			int d = min(base.cols, base.rows) / 2;
			int centerX = (base.cols - 1) / 2;
			int centerY = (base.rows - 1) / 2;
			int deltaX = (x - centerX) / d;
			int deltaY = (y - centerY) / d;
			double dstR = sqrt(deltaX * deltaX + deltaY * deltaY);
			double srcR = (outer * dstR * dstR * dstR + inner * dstR * dstR
					+ uniform * dstR + scale) * dstR;
			double factor = abs(dstR / srcR);
			int srcX = centerX + (deltaX * factor * d);
			int srcY = centerY + (deltaY * factor * d);
			vec.push_back(srcY * base.cols + srcX);
		}
	}
	return Mat(vec);
}
Mat bladArray(Mat base) {
	std::vector<float> vec;
	for (int x = 0; x < base.cols; x++) {
		for (int y = 0; y < base.rows; y++) {
			vec.push_back(y * base.cols + x);
		}
	}
	return Mat(vec);
}

 Mat warp(Mat input) {
	if (!f) {
		fish = genfish(input);
		f = true;
	}
	if (!b) {
		base = bladArray(input);
		b = true;
	}

	Mat ret;
	remap(input, ret, base, base,  0, BORDER_CONSTANT, Scalar());
	cout<<"ä"<< endl;;


	return ret;
}


Mat bleur(Mat input) {
	Mat ret;

	Mat element = getStructuringElement(2, Size(2 * 4 + 1, 2 * 4 + 1),
			Point(4, 4));

	erode(input, ret, element);
	return ret;
}
Mat sketch2 (Mat input){
	Mat invert, ret;
	bitwise_not(input,invert);
	GaussianBlur(invert, ret, Size(7, 7), 3, 3);

	add(ret, input, ret);
	return ret;

}
//sketch filter
Mat scetch(Mat input) {
	Mat ret;
	cvtColor(input, ret, CV_BGR2GRAY);
	GaussianBlur(ret, ret, Size(7, 7), 3, 3);
	Canny(ret, ret, 0, 32, 3);
	bitwise_not(ret, ret);
	return ret;

}
//applies filters
Mat filter(Mat input) {
	Mat out = sketch2(input);

	cout<<"size"<<out.cols<<out.rows<< endl;;


	return out;
}

void output(Mat left, Mat right) {
	Mat imgResult(left.rows, 2 * left.cols+spacing, left.type());

	Mat roiImgResult_Left = imgResult(Rect(0, 0, left.cols, left.rows));
	Mat roiImgResult_Right = imgResult(
			Rect(left.cols+spacing, 0, right.cols, right.rows));

	Mat roiImg1 = left(Rect(0, 0, left.cols, left.rows));
	Mat roiImg2 = right(Rect(0, 0, right.cols, right.rows));
	//Mat roiImg  = Rect(0,0,spacing,right.rows);

	roiImg1.copyTo(roiImgResult_Left); //Img1 will be on the left of imgResult
	roiImg2.copyTo(roiImgResult_Right); //Img2 will be on the right of imgResult
	resize(imgResult,imgResult,imagesize);
	imshow("Final imgage", imgResult);
	cv::moveWindow("Final imgage",screenx,screeny);



}
int main(int, char**) {
	VideoCapture cap(0); //open cam
	if (!cap.isOpened())
		return -1;

	Mat edges;
	namedWindow("edges", 1);
	while (true) {
		Mat frame;
		cap >> frame; // get a new frame from camera
		Mat result = filter(frame);
		output(result, result);

		if (waitKey(1) >= 0)
			break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
