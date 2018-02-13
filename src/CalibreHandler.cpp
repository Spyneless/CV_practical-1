
#include "CalibreHandler.h"

/*  Function Name: (initialSetting)
	Description:
	1. Setting camera data file path and resolution of video streaming.
	2. retrive the camera matrix and distortion coefficients.  (readCameraParam)
	3. Add 3D corner points (addThreeDimCornerPoints)
	4. Add 3D cube points (addDrawingPoints)
	5. Video start (videoStart)
*/
void CalibreHandler::initialSetting() {
	outputFileName = "out_camera_data.xml"

	
	Size m_borderSize = Size(9, 6);
	video_Width = 300;
	video_Height = 400;
	
	//Only for static photo testing. 
	m_filenames.push_back("img_list/1.jpg");
	m_filenames.push_back("img_list/2.jpg");
	m_filenames.push_back("img_list/3.jpg");
	m_filenames.push_back("img_list/4.jpg");
	
	readCameraParam();
	addThreeDimCornerPoints();
	addDrawingPoints();
	videoStart();
}



void CalibreHandler::readCameraParam() {
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
}


void addThreeDimCornerPoints() {
	for (int i = 0; i<m_borderSize.height; i++) {
		for (int j = 0; j<m_borderSize.width; j++) {
			threeDimCornerPoints.push_back(Point3f(i, j, 0.0f));
		}
	}
}

void addDrawingPoints() {
	objpts.push_back(Point3f(0, 0, 0));
	objpts.push_back(Point3f(0, 2, 0));  //X
	objpts.push_back(Point3f(2, 0, 0));  //Y
	objpts.push_back(Point3f(0, 0, 2));  //Z
	objpts.push_back(Point3f(0, 2, 2));
	objpts.push_back(Point3f(2, 2, 2));
	objpts.push_back(Point3f(2, 0, 2));
	objpts.push_back(Point3f(2, 2, 0));
}



void videoStart(const double &video_Width, const double &video_Height) {
	// open the default camera
	VideoCapture cap(0);
	// check if opened
	if (!cap.isOpened())  
		return -1;
	//Frame resolution 
	cap.set(CAP_PROP_FRAME_WIDTH, video_Width);
	cap.set(CAP_PROP_FRAME_HEIGHT, video_Height);
	cout << width << endl;
	cout << height << endl;
}

/* Function Name: (addTwoDimCornerPoints)
   Description:  
   Corners

*/

void addTwoDimCornerPoints() {
	findChessboardCorners(image, m_borderSize, TwoDimCornerPoints);  //2D corner points from the image.
	if ((TwoDimCornerPoints.size() == (m_borderSize.height * m_borderSize.width)))
	{
		drawChessboardCorners(image, m_borderSize, Mat(TwoDimCornerPoints), 1);
		mappingAndDrawing();
	}
}

/* Function Name: (mappingAndDrawing)
   Description:
   1. Finding rvec & tvec from 2D & 3D corner points
   2. Projecting Cube points into image points
   3. Drawing Cube. 
*/

void mappingAndDrawing() {
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






