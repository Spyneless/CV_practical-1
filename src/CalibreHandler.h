#pragma once


#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdio>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

class CalibreHandler {
private:
	const string outputFileName = "out_camera_data.xml";
	Mat cameraMatrix, disCoffs;
	double video_Width;
	double video_Height;
	vector<string> m_filenames;
	Size m_borderSize = Size(9, 6);

	Mat image;
	vector<Point3f> threeDimCornerPoints;
	vector<Point2f> twoDimCornerPoints;
	vector<Point3f> objpts;
	//Mat rvec, tvec;
	//vector<Point2f> imgpts;

public:
	void readCameraParam();
	void initialSetting(const double &res_width, const double &res_height);
	void videoStart(const double &video_Width, const double &video_Height);
	void addThreeDimCornerPoints();
	void addDrawingPoints();
	void addTwoDimCornerPoints();
	void mappingAndDrawing();

};

