//Written by  Kyle Hounslow 2013

// modified by: Ahmad Kaifi, Hassan Althobaiti

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include "Object.h"


//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH / 1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

//The following for canny edge detec
Mat dst, detected_edges;
Mat src, src_gray;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
const char* window_name = "Edge Map";

//Global vector where we're gonna send which leds turn on/off
float ledBelt[16];
const int distMax = 800;

void on_trackbar(int, void*)
{//This function gets called whenever a
 // trackbar position is changed

}

string intToString(int number) {

	std::stringstream ss;
	ss << number;
	return ss.str();
}

void createTrackbars() {
	//create window for trackbars
	namedWindow(trackbarWindowName, 0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN", H_MIN);
	sprintf(TrackbarName, "H_MAX", H_MAX);
	sprintf(TrackbarName, "S_MIN", S_MIN);
	sprintf(TrackbarName, "S_MAX", S_MAX);
	sprintf(TrackbarName, "V_MIN", V_MIN);
	sprintf(TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);
}

Point getPointingAngle(Object centralPoint, vector<Point> contours) {
	if (contours.size() != 3) return NULL;
	Point extremePoint;
	int distanceP0 = sqrt(pow(contours[0].x - centralPoint.getXPos(), 2) + pow(contours[0].y - centralPoint.getYPos(), 2));
	int distanceP1 = sqrt(pow(contours[1].x - centralPoint.getXPos(), 2) + pow(contours[1].y - centralPoint.getYPos(), 2));
	int distanceP2 = sqrt(pow(contours[2].x - centralPoint.getXPos(), 2) + pow(contours[2].y - centralPoint.getYPos(), 2));


	//Calc maximum distance
	int temp;
	int distTemp;
	if (distanceP0 > distanceP1) {
		extremePoint = contours[0];
		distTemp = distanceP0;
		temp = 0;
	}
	else {
		extremePoint = contours[1];
		distTemp = distanceP0;
		temp = 1;
	}
	if (distTemp > distanceP2) {
		extremePoint = contours[temp];

	}

	else {
		extremePoint = contours[2];
	}


	return extremePoint;
}

void drawObject(vector<Object> theObjects, Mat &frame, Mat &temp, vector< vector<Point> > contours, vector<Vec4i> hierarchy) {

	for (int i = 0; i<theObjects.size(); i++) {
		cv::drawContours(frame, contours, i, theObjects.at(i).getColor(), 3, 8, hierarchy);
		cv::circle(frame, cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos()), 5, theObjects.at(i).getColor());
		cv::putText(frame, intToString(theObjects.at(i).getXPos()) + " , " + intToString(theObjects.at(i).getYPos()), cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos() + 20), 1, 1, theObjects.at(i).getColor());
		cv::putText(frame, theObjects.at(i).getType(), cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos() - 20), 1, 2, theObjects.at(i).getColor());
		if (contours[i].size() == 3) {
			Point dir = getPointingAngle(theObjects.at(i), contours[i]);
			cv::circle(frame, dir, 5, theObjects.at(i).getColor());
		}

	}
}

void drawObject(vector<Object> theObjects, Mat &frame) {

	for (int i = 0; i<theObjects.size(); i++) {

		cv::circle(frame, cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos()), 10, cv::Scalar(0, 0, 255));
		cv::putText(frame, intToString(theObjects.at(i).getXPos()) + " , " + intToString(theObjects.at(i).getYPos()), cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos() + 20), 1, 1, Scalar(0, 255, 0));
		cv::putText(frame, theObjects.at(i).getType(), cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos() - 30), 1, 2, theObjects.at(i).getColor());
	}
}

void morphOps(Mat &thresh) {

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);

	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);
}



void trackFilteredObject(Mat threshold, Mat HSV, Mat &cameraFeed)
{
	vector <Object> objects;
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects<MAX_NUM_OBJECTS)
		{
			for (int index = 0; index >= 0; index = hierarchy[index][0])
			{
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if (area>MIN_OBJECT_AREA)
				{
					Object object;

					object.setXPos(moment.m10 / area);
					object.setYPos(moment.m01 / area);



					objects.push_back(object);

					objectFound = true;

				}
				else objectFound = false;
			}
			//let user know you found an object
			if (objectFound == true)
			{
				//draw object location on screen
				drawObject(objects, cameraFeed);
			}
		}
		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
}

vector<Point> trackPlayer(Mat &cameraFeed) {
	Mat threshold;
	Mat HSV;
	Object theObject("blue");

	//first find blue objects
	cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
	cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
	inRange(HSV, theObject.getHSVmin(), theObject.getHSVmax(), threshold);
	morphOps(threshold);

	vector <Object> objects;
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects<MAX_NUM_OBJECTS) {
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if (area>MIN_OBJECT_AREA) {

					Object object;

					object.setXPos(moment.m10 / area);
					object.setYPos(moment.m01 / area);
					object.setType(theObject.getType());
					object.setColor(theObject.getColor());

					objects.push_back(object);

					objectFound = true;

				}
				else objectFound = false;
			}
			//let user know you found an object
			if (objectFound == true) {
				//draw object location on screen

				//Approx each cotour object to a geometric figure
				for (int i = 0; i<contours.size(); i++)
					cv::approxPolyDP(contours[i], contours[i], 0.1*cv::arcLength(contours[i], true), true);

				Object playerP0 = objects[0];
				Point dir = getPointingAngle(playerP0, contours[0]);

				vector<Point> player;
				player.push_back(Point(playerP0.getXPos(), playerP0.getYPos()));
				player.push_back(dir);
				return player;


			}

		}
		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}

	//No return value
	vector<Point> voidPlayer;
	voidPlayer.push_back(Point(0, 0));
	voidPlayer.push_back(Point(0, 0));
	return voidPlayer;
}

void trackFilteredObject(Object theObject, Mat threshold, Mat HSV, Mat &cameraFeed) {

	vector <Object> objects;
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	//vector< vector<Point> > newContours;




	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects<MAX_NUM_OBJECTS) {
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if (area>MIN_OBJECT_AREA) {

					Object object;

					object.setXPos(moment.m10 / area);
					object.setYPos(moment.m01 / area);
					object.setType(theObject.getType());
					object.setColor(theObject.getColor());

					objects.push_back(object);

					objectFound = true;

				}
				else objectFound = false;
			}
			//let user know you found an object
			if (objectFound == true) {
				//draw object location on screen

				//Approx each cotour object to a geometric figure
				for (int i = 0; i<contours.size(); i++)
					cv::approxPolyDP(contours[i], contours[i], 0.1*cv::arcLength(contours[i], true), true);


				drawObject(objects, cameraFeed, temp, contours, hierarchy);
			}

		}
		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
}

void calibrateLEDs(double angle, double distance)
{
	float intense = 1 - (distance / distMax);

	if ((angle > 0 && angle <= 11'25) || (angle > 348'75 && angle <= 360)) {
		ledBelt[0] = intense;
	}
	else if (angle > 11'25 && angle <= 33'75) {
		ledBelt[1] = intense;
	}
	else if (angle > 33'75 && angle <= 56'25) {
		ledBelt[2] = intense;
	}
	else if (angle > 56'25 && angle <= 78'75) {
		ledBelt[3] = intense;
	}
	else if (angle > 78'75 && angle <= 101'25) {
		ledBelt[4] = intense;
	}
	else if (angle > 101'25 && angle <= 123'75) {
		ledBelt[5] = intense;
	}
	else if (angle > 123'75 && angle <= 146'25) {
		ledBelt[6] = intense;
	}
	else if (angle > 146'25 && angle <= 168'75) {
		ledBelt[7] = intense;
	}
	else if (angle > 168'75 && angle <= 191'25) {
		ledBelt[8] = intense;
	}
	else if (angle > 191'25 && angle <= 213'75) {
		ledBelt[9] = intense;
	}
	else if (angle > 213'75 && angle <= 236'25) {
		ledBelt[10] = intense;
	}
	else if (angle > 236'25 && angle <= 258'75) {
		ledBelt[11] = intense;
	}
	else if (angle > 258'75 && angle <= 281'25) {
		ledBelt[12] = intense;
	}
	else if (angle > 281'25 && angle <= 303'75) {
		ledBelt[13] = intense;
	}
	else if (angle > 303'75 && angle <= 326'25) {
		ledBelt[14] = intense;
	}
	else if (angle > 326'25 && angle <= 348'75) {
		ledBelt[15] = intense;
	}
}

int main(int argc, char* argv[])
{
	Mat cameraFeed;
	vector<vector<Point>>rectcount;
	VideoCapture capture;
	capture.open(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	waitKey(1000);
	while (1) {
		capture.read(cameraFeed);
		src = cameraFeed;
		if (!src.data) return -1;

		vector<Point> player = trackPlayer(cameraFeed);
		cv::line(cameraFeed, player.at(0), player.at(1), Scalar(110, 220, 0), 2, 8);

		imshow(windowName, cameraFeed);
		waitKey(30);
	}
	return 0;
}