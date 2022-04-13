#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int lpos, rpos;



//void keep_brightness(frame, int target);
//void noise_handling(frame);

int pos_hue1 = 10, pos_hue2 = 30, pos_sat1 = 100, pos_sat2 = 255;
Scalar lowerb(pos_hue1, pos_sat1, 100);
Scalar upperb(pos_hue2, pos_sat2, 255);
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
	
	vector<Point> pts(4);
	int npt[] = { 4 };
	pts.push_back(Point(0, 370));
	pts.push_back(Point(640, 370));
	pts.push_back(Point(640, 430));
	pts.push_back(Point(0, 430));

	Mat mask = Mat::zeros(h, w, CV_8UC1);
	fillPoly(mask, pts, 255);
	
	Mat frame, edge;
	while (true) {
		Mat gray,  frame_hsv, frame_edge;
		cap >> frame;
		
		//line(frame, pts[0], pts[1], Scalar(255, 0, 0), 2);
		//line(frame, pts[1], pts[2], Scalar(255, 0, 0), 2);
		//line(frame, pts[2], pts[3], Scalar(255, 0, 0), 2);
		//line(frame, pts[3], pts[0], Scalar(255, 0, 0), 2);

		if (frame.empty()) {
			cerr << "Empty frame!" << endl;
			break;
		}

		// HSV 색공간변환
		cvtColor(frame, frame_hsv, COLOR_BGR2HSV);

		// 블러 
		GaussianBlur(frame, frame, Size(0, 0), (double)1);

		// 밝기유지
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		float alpha = 1.0f;
		int m = (int)(mean(gray, mask)[0]);
		Mat dst = gray + (140 - m) * alpha;

		// 이진화
		Mat dst_threshold;
		threshold(dst, dst_threshold, 125, 255, THRESH_BINARY);

		//캐니
		Canny(dst_threshold, frame_edge, 50, 150);
		frame_edge.copyTo(edge, mask);

		// 허프
		vector<Vec4i> lines1;
		HoughLinesP(edge, lines1, 1, CV_PI / 180, 30, 30, 10);

		for (size_t i = 0; i < lines1.size(); i++) {
			Vec4i l = lines1[i];

			if (abs(l[3] - l[1]) > 10) {
				line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, LINE_AA);
			}
		}

		// 노이즈처리

		imshow("frame", frame);
		imshow("frame_edge", frame_edge);
		imshow("dst_threshold", dst_threshold);

		if (waitKey(1) == 27) //1ms마다 확인
			break;
	}

	cap.release();
	destroyAllWindows();
}