#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int lpos, rpos;
Mat frame, edge;


//void keep_brightness(frame, int target);
//void noise_handling(frame);

int main()
{
	VideoCapture cap;
	cap.open("subProject.avi");

	if (!cap.isOpened()) {
		cerr << "Camera open failed!" << endl;
		return -1;
	}

	int w = cvRound(cap.get(CAP_PROP_FRAME_WIDTH));
	int h = cvRound(cap.get(CAP_PROP_FRAME_HEIGHT));
	double fps = cap.get(CAP_PROP_FPS);
	
	while (true) {
		
		cap >> frame;

		if (frame.empty()) {
			cerr << "Empty frame!" << endl;
			break;
		}
		// 블러 
 
		// 밝기유지
		 
		// HSV 색공간변환
		
		// 이진화
		
		// 엣지검출
		Canny(frame, edge, 50, 150); 
		// 허프
		
		// 노이즈처리

		imshow("frame", frame);
		imshow("edge", edge);

		if (waitKey(1) == 27) //1ms마다 확인
			break;
	}

	cap.release();
	destroyAllWindows();
}