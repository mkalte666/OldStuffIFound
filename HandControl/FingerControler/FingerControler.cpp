// FingerControler.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include <Windows.h>

cv::Mat getColor(cv::Mat &inframe, int r, int g, int b, int tolerance);
void onMouse( int event, int x, int y, int, void* );
cv::Vec2f getPosition(cv::Mat &inframe);
std::vector<cv::Vec2f> GetMultiplePositions(cv::Mat &inframe, bool fast_calculate=true, int sensity = 2);
float CalcAngle(cv::Vec2f first, cv::Vec2f second);
cv::Mat highCurve(cv::Mat&inframe);

cv::Vec3b g_selectedColor;
bool g_track;



int _tmain(int argc, _TCHAR* argv[])
{
		g_track = false;
	cv::VideoCapture capture(0);

	cv::Mat frame;
	cv::namedWindow("Image");
	cv::namedWindow("Control");
	cv::setMouseCallback("Control",onMouse, &frame); 
	while(true) {
		capture >> frame;
		
		cv::Mat image = getColor(frame, g_selectedColor[0], g_selectedColor[1], g_selectedColor[2], 50);
		cv::Mat greyImage;
		cvtColor(image, greyImage, CV_BGR2GRAY);
		cv::Canny(greyImage, greyImage, 30, 30 * 3, 3);

		cv::imshow("Image", image);
		cv::imshow("testcurve", greyImage);
		cv::imshow("Control", frame);
		cv::waitKey(10);
	}
	return 0;


}

cv::Mat getColor(cv::Mat &inframe, int r, int g, int b, int tolerance)
{

	int w = inframe.cols;
	
	int h = inframe.rows;
	cv::Mat outmap;
	inframe.copyTo(outmap);
	outmap = cv::Scalar(0);

	int upper_r = r+tolerance;
	int lower_r = r-tolerance;
	int upper_g = g+tolerance;
	int lower_g = g-tolerance;
	int upper_b = b+tolerance;
	int lower_b = b-tolerance;

	//Compare Colors
	for(int i = 0; i<w*h*3; i+=3) {
		if(inframe.data[i] <= upper_r && inframe.data[i] >= lower_r) {
			if(inframe.data[i+1] <= upper_g && inframe.data[i+1] >= lower_g) {
				if(inframe.data[i+2] <= upper_b && inframe.data[i+2] >= lower_b) {
					int bw = 255*3 -abs(inframe.data[i]-r) - abs(inframe.data[i]-r) - abs(inframe.data[i]-r);
					bw /= 3;
					outmap.data[i] = bw;
					outmap.data[i + 1] = bw;
					outmap.data[i + 2] = bw;
				}
			}
		}
	}
	return outmap;
}

cv::Mat highCurve(cv::Mat&inframe) {
	cv::Mat outmat;
	int w = inframe.cols;
	int h = inframe.rows;
	inframe.copyTo(outmat);
	outmat = cv::Scalar(0);
	for (int x = 0; x < w; x++) {
		float count = 0;
		for (int y = 0; y < h; y++) {
			cv::Point pos(x, y);
			count += inframe.at<unsigned char>(pos) > 100 ? 1 : 0;
		}
		if (count > h-1) count = h-1;
		if (count < 0) count = 0;
		cv::Point pos(x,h-1-int(count));
		outmat.at<cv::Vec3b>(pos) = cv::Vec3b(255, 255, 255);
	}
	return outmat;
}

void onMouse( int event, int x, int y, int b, void* userdata) {
	if(event==CV_EVENT_LBUTTONDOWN) {
		cv::Mat frame = *(cv::Mat*)userdata;
		g_selectedColor = frame.at<cv::Vec3b>(cv::Point(x,y));
		std::cout << "selecting color: " << (int)g_selectedColor[0] << " " << (int)g_selectedColor[1] << " " << (int)g_selectedColor[2] << std::endl;
	}
	if(!g_track && CV_EVENT_RBUTTONDOWN==event) g_track = true;
	else if(g_track && CV_EVENT_RBUTTONDOWN==event) g_track = false;
}


