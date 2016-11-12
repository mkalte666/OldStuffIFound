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
		
		cv::imshow("Image", image);
		cv::Vec2f pos = getPosition(image);
		//std::cout << "Marker at: " << pos[0] << " " << pos[1] << std::endl;
		std::vector<cv::Vec2f> positions = GetMultiplePositions(image, false, 7);
		if(positions.size()==2) {
			cv::Vec2f first = positions[0]; 
			cv::Vec2f second = positions[1];
			if(positions[0][0]>positions[1][0]) {
				first = positions[1]; 
				second = positions[0];
			}
			if(first[1]<second[1]) std::cout << "Lings gedreht! 1: ["<< first[0] << "|"<<first[1]<<"] 2: [" << second[0] << "|" << second[1] << "]\n";
			else std::cout << "Rechts gedreht! 1: ["<< first[0] << "|"<<first[1]<<"] 2: [" << second[0] << "|" << second[1] << "]\n";

			//Calculate angle
			std::cout << "Winkel: " << CalcAngle(first, second)-90 << "\n";

		}

		if(pos[0] != -1 && pos[1] != -1)
			if(g_track)
				SetCursorPos(pos[0]*1920, pos[1]*1080);

			
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
					outmap.data[i+1] = bw;
					outmap.data[i+2] = bw;
				}
			}
		}
	}
	return outmap;
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

cv::Vec2f getPosition(cv::Mat &inframe)
{
	int w = inframe.cols;
	int h = inframe.rows;
	int posnum = 0;
	cv::Vec2f pos;

	for(int i = 0; i < w; i++) {
		for(int j = 0; j< h; j++) {
			cv::Vec3b bw = inframe.at<cv::Vec3b>(cv::Point(i,j));
			if(bw[0]>50&&bw[1]>50&&bw[2]>50) {
				posnum++;
				pos[0] += (float)i/(float)w;
				pos[1] += (float)j/(float)h;
			}
		}
	}
	if(posnum<=0) 
		return cv::Vec2f(-1.00f);

	pos[0] /= (float)posnum;
	pos[1] /= (float)posnum;
	pos[0] = 1 - pos[0];
	return pos;
}

//BLACK MAGIC
std::vector<cv::Vec2f> GetMultiplePositions(cv::Mat &inframe, bool fast_calculate, int sensity)
{
	std::vector<cv::Vec2f> ret;
	int harea_begin;
	int harea_end;
	bool harea_active = false;
	for(int hpos = 0; hpos<inframe.rows; hpos+=sensity) {
		bool line_found = false;
		for(int wpos = 0; wpos<inframe.cols; wpos+=sensity) {
			cv::Vec3b bw = inframe.at<cv::Vec3b>(cv::Point(wpos,hpos));
			if(bw[0]>50&&bw[1]>50&&bw[2]>50) {
				line_found = true;
				break;
			}
		}

		//Make the area
		if(line_found&&!harea_active) {
			harea_active = true;
			harea_begin = hpos;
		} else if (!line_found&&harea_active || harea_active&&hpos==inframe.rows) {
			harea_active = false;
			harea_end = hpos;
			//So we have an area. Lets split it into more areas
			int warea_begin;
			int warea_end;
			bool warea_active = false;
			for(int wpos = 0; wpos<inframe.cols; wpos+=sensity) {
				bool line_found2 = false;
				for(int hpos2 = harea_begin; hpos2<harea_end; hpos2+=sensity) {
					cv::Vec3b bw = inframe.at<cv::Vec3b>(cv::Point(wpos,hpos2));
					if(bw[0]>50&&bw[1]>50&&bw[2]>50) {
						line_found2 = true;
						break;
					}
				}
				if(line_found2&&!warea_active) {
					warea_begin = wpos;
					warea_active = true;
				} else if(!line_found2&&warea_active) {
					warea_active = false;
					warea_end = wpos;
					//We have a area now. And we have 2 ways to calculate the pos: fast and slow. 
					cv::Vec2f pos;
					if(fast_calculate) {
						pos[0] = warea_begin+(warea_begin-warea_end)/2;
						pos[1] = harea_begin+(harea_begin-harea_end)/2;
						ret.push_back(pos);
					} else {
						int posnum = 0;
						for(int x = warea_begin; x<warea_end; x++) {
							for(int y = harea_begin; y<harea_end;y++) {
								cv::Vec3b bw = inframe.at<cv::Vec3b>(cv::Point(x,y));
								if(bw[0]>50&&bw[1]>50&&bw[2]>50) {
									pos[0] += x;
									pos[1] += y;
									posnum++;
								}
							}
						}
						if(posnum<=0)
							continue;
						pos[0] /= posnum;
						pos[1] /= posnum;

						ret.push_back(pos);
					}
				}

			}
		}
	}


	return ret;
}

float CalcAngle(cv::Vec2f first, cv::Vec2f second)
{
	cv::Vec2f first_to_second_half = (second-first)/2;
	return acos(first_to_second_half[1]/sqrt(first_to_second_half[0]*first_to_second_half[0]+first_to_second_half[1]*first_to_second_half[1]))*180/3.14;
}