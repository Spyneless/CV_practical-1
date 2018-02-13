
#include "CalibreHandler.h"

/*  Function Name: (initialSetting)
	Description:
	1. Setting camera data file path and resolution of video streaming.
	2. retrive the camera matrix and distortion coefficients.  
	3. Add 3D corner points 
	4. Add 3D cube points 
	5. Video start 
*/


void CalibreHandler::initialSetting() {
	//Basic Settting
	outputFileName = "out_camera_data.xml";
	m_borderSize = Size(9, 6);
	video_Width = 480;
	video_Height = 600;
	//Get CameraMat & DistCoeffs.
	readCameraParam();
	//Add 3D corner points
	addThreeDimCornerPoints();
	//Add 3D cube points
	addDrawingPoints();
	//Open webcam and start to catch each frame
	videoStart();
}
/*  Function Name: readCameraParam
	Description:
	1.retrive the camera matrix and distortion coefficients. 
*/

bool CalibreHandler::readCameraParam() {
	FileStorage fs(outputFileName, FileStorage::READ);
	if (!fs.isOpened())
	{
		cout << "Could not open the configuration file: \"" << outputFileName << "\"" << endl;
		return 0;
	}
	
	fs["distortion_coefficients"] >> disCoffs;
	fs["camera_matrix"] >> cameraMatrix;
	return 1;
}

/*  Function Name: addThreeDimCornerPoints
	Description:
	1. Add 3D corner points into "threeDimCornerPoints"
*/
void CalibreHandler::addThreeDimCornerPoints() {
	for (int i = 0; i<m_borderSize.height; i++) {
		for (int j = 0; j<m_borderSize.width; j++) {
			threeDimCornerPoints.push_back(Point3f(i, j, 0.0f));
		}
	}
}

/*  Function Name: addDrawingPoints
	Description:
	1. Add 3D cube points into "objpts"
*/
void CalibreHandler::addDrawingPoints() {
	objpts.push_back(Point3f(0, 0, 0));
	objpts.push_back(Point3f(0, 2, 0));
	objpts.push_back(Point3f(2, 0, 0));
	objpts.push_back(Point3f(0, 0, 2));
	objpts.push_back(Point3f(0, 2, 2));
	objpts.push_back(Point3f(2, 2, 2));
	objpts.push_back(Point3f(2, 0, 2));
	objpts.push_back(Point3f(2, 2, 0));

	objpts.push_back(Point3f(0, 4, 0));  //X
	objpts.push_back(Point3f(4, 0, 0));  //Y
	objpts.push_back(Point3f(0, 0, 4));  //Z

}

/*  Function Name: videoStart
	Description:
	1. Open webcam and start to loop each frame.
	2. Find 2D corner points.
	3. mapping cube points into image by mappingAndDrawing func.
*/

bool CalibreHandler::videoStart() {
	// open the default camera
	
	VideoCapture cap(0);
	// check if opened
	if (!cap.isOpened()) {
		cout << "Could not open the webcam" << endl;
		return 0;
	}
	// Set resolution 
	cap.set(CAP_PROP_FRAME_WIDTH, video_Width);
	cap.set(CAP_PROP_FRAME_HEIGHT, video_Height);
	//Looping each frame
	for (;;)
	{
		cap >> image;
		//Find 2D corner points
		twoDimCornerPoints.clear();
		findChessboardCorners(image, m_borderSize, twoDimCornerPoints);
			//Check if the size of found points match the border size
			if ((twoDimCornerPoints.size() == (m_borderSize.height * m_borderSize.width)))
			{
				//Draw corner points
				drawChessboardCorners(image, m_borderSize, Mat(twoDimCornerPoints), 1);
				//Mapping cube into image
				mappingAndDrawing();
			}
		imshow("OpenCV", image);
			if (waitKey(30) >= 0) break;
	}
	return 1;
}

/* Function Name: mappingAndDrawing
   Description:
   1. Finding rvec & tvec from 2D & 3D corner points
   2. Projecting Cube points into image points
   3. Drawing Cube by these image points. 
*/

void CalibreHandler::mappingAndDrawing() {
	//Finding rvec & tvec from corner points in 2D and 3D
	Mat rvec, tvec;
	vector<Point2f> imgpts;
	solvePnP(threeDimCornerPoints, twoDimCornerPoints, cameraMatrix, disCoffs, rvec, tvec, false, 0);
	
	//Projecting Cube points into image points
	projectPoints(objpts, rvec, tvec, cameraMatrix, disCoffs, imgpts, noArray(), 0);

	//Drawing a cube
	//3-axis 
	arrowedLine(image, imgpts[0], imgpts[8], Scalar(255, 0, 0), 4, 8, 0, 0.1);
	arrowedLine(image, imgpts[0], imgpts[9], Scalar(0, 255, 0), 4, 8, 0, 0.1);
	arrowedLine(image, imgpts[0], imgpts[10], Scalar(0, 0, 255), 4, 8, 0, 0.1);
	//lines of cube
	line(image, imgpts[0], imgpts[1], Scalar(0, 255, 100), 2, 8, 0);
	line(image, imgpts[0], imgpts[2], Scalar(0, 255, 100), 2, 8, 0);
	line(image, imgpts[0], imgpts[3], Scalar(0, 255, 100), 2, 8, 0);
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






