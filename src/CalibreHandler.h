
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
	string outputFileName;
	Mat cameraMatrix, disCoffs;
	double video_Width;
	double video_Height;
	//vector<string> m_filenames;
	Size m_borderSize;

	Mat image;
	vector<Point3f> threeDimCornerPoints;
	vector<Point2f> twoDimCornerPoints;
	vector<Point3f> objpts;

public:
	bool readCameraParam();
	void initialSetting();
	bool videoStart();
	void addThreeDimCornerPoints();
	void addDrawingPoints();
	void mappingAndDrawing();

};

