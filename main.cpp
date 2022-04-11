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
		// �� 
 
		// �������
		 
		// HSV ��������ȯ
		
		// ����ȭ
		
		// ��������
		Canny(frame, edge, 50, 150); 
		// ����
		
		// ������ó��

		imshow("frame", frame);
		imshow("edge", edge);

		if (waitKey(1) == 27) //1ms���� Ȯ��
			break;
	}

	cap.release();
	destroyAllWindows();
}