#include <iostream>
#include <fstream>
#include <random>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


int get_pos(vector<Vec4i>& lines);
int get_pos_ransac(vector<Vec4i>& lines, Mat& frame, double inlier_thres = 12.0, int pos_height = 400);

template <typename T>
void get_line_param(T& p1, T& p2, double& m, double& n);

random_device rd;
mt19937 gen(rd());

int main()
{
	VideoCapture cap;
	VideoWriter vriter;
	ofstream outfile;

	cap.open("subProject.avi");

	if (!cap.isOpened()) {
		cerr << "Camera open failed!" << endl;
		return -1;
	}

	int w = cvRound(cap.get(CAP_PROP_FRAME_WIDTH));
	int h = cvRound(cap.get(CAP_PROP_FRAME_HEIGHT));
	double fps = cap.get(CAP_PROP_FPS);

	vriter.open("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(w, h), true);
	
	vector<Point> pts(4);
	int npt[] = { 4 };
	pts.push_back(Point(0, 360));
	pts.push_back(Point(640, 360));
	pts.push_back(Point(640, 440));
	pts.push_back(Point(0, 440));

	Mat mask = Mat::zeros(h, w, CV_8UC1);
	fillPoly(mask, pts, 255);
	
	Mat frame, edge;
	int n_frame = 1;
	vector<pair<int, int>> history;

	while (true) {
		Mat gray, frame_hsv, frame_edge;
		cap >> frame;

		if (frame.empty()) {
			cerr << "Empty frame!" << endl;
			break;
		}

		// 블러 
		GaussianBlur(frame, frame, Size(0, 0), (double)1);

		// 밝기유지
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		float alpha = 1.0f;
		int m = (int)(mean(gray, mask)[0]);

		Mat dst = gray + (140 - m) * alpha;

		// 이진화
		Mat dst_threshold;
		threshold(dst, dst_threshold, 100, 255, THRESH_BINARY);

		//캐니
		Canny(dst_threshold, frame_edge, 50, 150);
		frame_edge.copyTo(edge, mask);

		// 허프
		vector<Vec4i> lines1;
		HoughLinesP(edge, lines1, 1, CV_PI / 180, 30, 30, 10);

		vector<Vec4i> l_lines, r_lines;
		for (size_t i = 0; i < lines1.size(); i++) {
			Vec4i l = lines1[i];
		
			double m = double(l[1] - l[3]) / (l[0] - l[2]);
			if (abs(l[3] - l[1]) > 10) {
				if (m < 0 && l[2] < 400) {
					l_lines.push_back(l);
				}
				else if(m > 0 && l[0] > 240){
					r_lines.push_back(l);
				}
			}
		}

		for (size_t i = 0; i < l_lines.size(); i++) {
			Vec4i l = l_lines[i];
			line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, LINE_AA);
		}
		for (size_t i = 0; i < r_lines.size(); i++) {
			Vec4i l = r_lines[i];
			line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 2, LINE_AA);
		}

		int lpos, rpos;

		if (l_lines.size() == 0)
			lpos = 0;
		else
			lpos = get_pos_ransac(l_lines, frame);
		
		if (r_lines.size() == 0)
			rpos = 640;
		else
			rpos = get_pos_ransac(r_lines, frame);
	
		rectangle(frame, Rect(Point(lpos - 10, 390), Point(lpos + 10, 410)), Scalar(0, 255, 0), 2, LINE_AA);
		rectangle(frame, Rect(Point(rpos - 10, 390), Point(rpos + 10, 410)), Scalar(0, 255, 0), 2, LINE_AA);
		
		int pos = cvRound(cap.get(CAP_PROP_POS_FRAMES));
		String text = format("Frame number: %d", pos);
		putText(frame, text, Point(20, 50), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 1, LINE_AA);

		vriter << frame;

		if (waitKey(1) == 27) //1ms마다 확인
			break;

		if (n_frame % 30 == 0) {
			pair<int, int> temp = make_pair(lpos, rpos);
			history.push_back(temp);
		}
		n_frame++;
	}

	outfile.open("test.csv", ios::out);

	//csv파일에 저장
	for (int j = 0; j < history.size(); j++)
	{
		outfile << history[j].first << "," << history[j].second << endl;
	}

	outfile.close();
	cap.release();
	destroyAllWindows();
}

int get_pos(vector<Vec4i>& lines) {
	double mean_m = 0.0, mean_n = 0.0;
	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		double m, n;
		Point p1(l[0], l[1]);
		Point p2(l[2], l[3]);
		get_line_param(p1, p2, m, n);
		mean_m += m;
		mean_n += n;
	}
	mean_m /= lines.size();
	mean_n /= lines.size();

	int pos = cvRound((400 - mean_n) / mean_m);
	return pos;
}

int get_pos_ransac(vector<Vec4i>& lines, Mat& frame, double inlier_thres, int pos_height)
{
	vector<Point2d> points;

	// gather dots from lines
	for (size_t i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		pair<int, int> d_vec(l[2] - l[0], l[3] - l[1]);
		double length = sqrt(pow(d_vec.first, 2) + pow(d_vec.second, 2));

	

		for (int j = 0; j <= length/2; j += 1) {
			double x = l[0] + (d_vec.first * 2 * j) / length;
			double y = l[1] + (d_vec.second * 2 * j) / length;
			points.emplace_back(Point2d(x, y));
		}
	}

	int max_count = -1;
	int max_p1, max_p2;

	for (int i = 0; i < 100; i++) {
		// select two points randomly
		uniform_int_distribution<int> dis(0, points.size() - 1);
		int p1 = dis(gen);
		int p2 = dis(gen);

		while (p1 == p2) {
			p2 = dis(gen);
		}
		// compute params
		double m, n;
		get_line_param(points[p1], points[p2], m, n);
	
		// varfication
		int count = 0;

		for (auto& point : points) {
			double distance = abs(m * point.x - point.y + n) / sqrt(m * m + 1);
			if (distance < inlier_thres) {
				count++;
			}
		}

		if (count > max_count) {
			max_p1 = p1;
			max_p2 = p2;
			max_count = count;
		}
	}

	line(frame, points[max_p1], points[max_p2], Scalar(0, 255, 0), 2, LINE_AA);

	// calculate pos
	double m, n;
	get_line_param(points[max_p1], points[max_p2], m, n);

	int pos = cvRound((pos_height - n) / m);
	return pos;
}


template <typename T>
void get_line_param(T& p1, T& p2, double& m, double& n) {
	m = (double)(p1.y - p2.y) / (p1.x - p2.x);
	n = -m * p1.x + p1.y;
}