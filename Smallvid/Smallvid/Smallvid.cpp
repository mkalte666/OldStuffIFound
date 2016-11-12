// Smallvid.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

#define RECORD_FRAMES	50

struct pixelblock
{
	Vec2i	pos;
	Vec3b	startcolor;
	Vec3b	endcolor;
	unsigned int blocksize;
	
	Vec3b CalcValue(int frameoffset) {
		if (frameoffset >= blocksize) return endcolor;
		if (frameoffset == 0) return startcolor;
		//Calculate pixel color

	}
};


int main(int argc, char* argv[])
{
	if (argc <= 1) {
		cout << "Please give me a file to open!" << endl;
		return 1;
	}

	cvNamedWindow("in");
	cvNamedWindow("out");

	std::vector<Mat>	in_recording;
	VideoCapture capture(argv[1]);
	Mat	frame;

	//Create a few frames to work with
	for (int i = 0; i < RECORD_FRAMES; i++) {
		capture >> frame;
		in_recording.push_back(frame);
		cvWaitKey(20);
	}




	return 0;
}



