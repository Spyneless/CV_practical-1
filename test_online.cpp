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
#include "CalibreHandler.h"

using namespace cv;
using namespace std;





int main()
{
	//Retriving CameraMatrix and DistortionCoffs. from the XML file
	///CalibreHandler calibre1;

	const string outputFileName = "out_camera_data.xml";
	FileStorage fs(outputFileName, FileStorage::READ);
	if (!fs.isOpened())
	{
		cout << "Could not open the configuration file: \"" << outputFileName << "\"" << endl;
		return -1;
	}
	Mat CameraMatrix, DisCoffs;
	Mat image;
	fs["distortion_coefficients"] >> DisCoffs;
	fs["camera_matrix"] >> CameraMatrix;

	//Set image files
	vector<string> m_filenames;
	Size m_borderSize = Size(9, 6);
	m_filenames.push_back("img_list/1.jpg");
	m_filenames.push_back("img_list/2.jpg");
	m_filenames.push_back("img_list/3.jpg");
	m_filenames.push_back("img_list/4.jpg");
	
	//3d
	vector<Point3f> ThreeDimCornerPoints;
	for (int i = 0; i<m_borderSize.height; i++) {
		for (int j = 0; j<m_borderSize.width; j++) {
			ThreeDimCornerPoints.push_back(Point3f(i, j, 0.0f));
		}
	}
	//object points of drawing the cube
	vector<Point3f> objpts;
	objpts.push_back(Point3f(0, 0, 0));
	objpts.push_back(Point3f(0, 2, 0));  //X
	objpts.push_back(Point3f(2, 0, 0));  //Y
	objpts.push_back(Point3f(0, 0, 2));  //Z
	objpts.push_back(Point3f(0, 2, 2));
	objpts.push_back(Point3f(2, 2, 2));
	objpts.push_back(Point3f(2, 0, 2));
	objpts.push_back(Point3f(2, 2, 0));
	//



	VideoCapture cap(0); // open the default camera

	if (!cap.isOpened())  // check if we succeeded
		return -1;
	//image obtained from streaming.
	cap.set(CAP_PROP_FRAME_WIDTH, 300);
	cap.set(CAP_PROP_FRAME_HEIGHT, 400);
	double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double	height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);



	cout << width << endl;
	cout << height << endl;

	for (;;)
	{
		cout << cap.get(CV_CAP_PROP_FPS) << endl;
		//Mat frame;
		cap >> image; // get a new frame from camera

	//build 2D corner points 
	vector<Point2f> TwoDimCornerPoints;
	//image = imread(m_filenames[3], CV_LOAD_IMAGE_COLOR);

		findChessboardCorners(image, m_borderSize, TwoDimCornerPoints);  //2D corner points from the image.
		if((TwoDimCornerPoints.size() == (m_borderSize.height * m_borderSize.width)))
		{
			drawChessboardCorners(image, m_borderSize, Mat(TwoDimCornerPoints), 1); // draw corner points on the image. 

			/*																		//build 3D corner points
			vector<Point3f> ThreeDimCornerPoints;
			for (int i = 0; i<m_borderSize.height; i++) {
				for (int j = 0; j<m_borderSize.width; j++) {
					ThreeDimCornerPoints.push_back(Point3f(i, j, 0.0f));
				}
			}
			//object points of drawing the cube
			vector<Point3f> objpts;
			objpts.push_back(Point3f(0, 0, 0));
			objpts.push_back(Point3f(0, 2, 0));  //X
			objpts.push_back(Point3f(2, 0, 0));  //Y
			objpts.push_back(Point3f(0, 0, 2));  //Z
			objpts.push_back(Point3f(0, 2, 2));
			objpts.push_back(Point3f(2, 2, 2));
			objpts.push_back(Point3f(2, 0, 2));
			objpts.push_back(Point3f(2, 2, 0));
			*/


			//Finding Rotation & Translation vectors from corner points in 2D and 3D
			Mat rvec, tvec;
			vector<Point2f> imgpts;
			solvePnP(ThreeDimCornerPoints, TwoDimCornerPoints, CameraMatrix, DisCoffs, rvec, tvec, false, 0);
			//Projecting targeted points of cube into image points. 
			projectPoints(objpts, rvec, tvec, CameraMatrix, DisCoffs, imgpts, noArray(), 0);

			//Drawing a cube

			//3-axis 
			arrowedLine(image, imgpts[0], imgpts[1], Scalar(255, 0, 0), 4, 8, 0, 0.1);
			arrowedLine(image, imgpts[0], imgpts[2], Scalar(0, 255, 0), 4, 8, 0, 0.1);
			arrowedLine(image, imgpts[0], imgpts[3], Scalar(0, 0, 255), 4, 8, 0, 0.1);
			//lines of cube
			line(image, imgpts[3], imgpts[4], Scalar(0, 255, 100), 2, 8, 0);
			line(image, imgpts[4], imgpts[1], Scalar(0, 255, 100), 2, 8, 0);
			line(image, imgpts[4], imgpts[5], Scalar(0, 255, 100), 2, 8, 0);
			line(image, imgpts[5], imgpts[7], Scalar(0, 255, 100), 2, 8, 0);
			line(image, imgpts[5], imgpts[6], Scalar(0, 255, 100), 2, 8, 0);
			line(image, imgpts[7], imgpts[1], Scalar(0, 255, 100), 2, 8, 0);
			line(image, imgpts[7], imgpts[2], Scalar(0, 255, 100), 2, 8, 0);
			line(image, imgpts[6], imgpts[3], Scalar(0, 255, 100), 2, 8, 0);
			line(image, imgpts[6], imgpts[2], Scalar(0, 255, 100), 2, 8, 0);

			}
		imshow("OpenCV", image);

		if (waitKey(10) >= 0) break;
	}

	return 0;
}

