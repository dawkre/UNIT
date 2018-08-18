#include "detection.h"
#include "cameras.h"
#include "calculations.h"
#include "structs.h"
#include "calibration.h"
#include "globals.h"
extern "C" {
#include "PTZF_control.h"
}

#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

/*Initialize devices*/
device left_cam {"/dev/ttyMXUSB0", 0, initializeDevice((char*)"/dev/ttyMXUSB0"), "21818297", {158, 90, 100, 0}  , {0, 0, 0, 0}};
device right_cam {"/dev/ttyMXUSB1", 1,	initializeDevice((char*)"/dev/ttyMXUSB1"), "21855432", {199, 90, 100, 0} , {0, 0, 0, 0}};

Cameras cameras;
// std::string cfg_file = "data/yolo-fe.cfg";
// std::string weights_file = "data/yolo-fe_final.weights";
std::string cfg_file = "data/yolov3-tiny-obj.cfg";
std::string weights_file = "data/yolov3-tiny-obj-3.weights";
// std::string cfg_file = "data/yolo-face.cfg";
// std::string weights_file = "data/yolo-face_final.weights";
// std::string cfg_file = "data/yolo-tiny.cfg";

int CALMAN_FRAME_WITHOUT_DETECTION = 30;

Detector detector(cfg_file, weights_file);
char key_pressed;

types tracked_type = face;
#include <atomic>
#include <thread>
#include <iostream>

void ReadCin(std::atomic<bool>& run)
{
	char buffer;

	while (run.load())
	{
		buffer = getch();
		if (buffer == '1') tracked_type = face;
		if (buffer == '2') tracked_type = left_eye;
		if (buffer == '3') tracked_type = right_eye;
	}
}
KalmanFilter generateKalman() {
	KalmanFilter Filter(4, 2, 0);
	Filter.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
	Filter.statePre.at<float>(0) = 0;        //Położenie x
	Filter.statePre.at<float>(1) = 0;        //Położenie y
	Filter.statePre.at<float>(2) = 0;        //Prędkosc x
	Filter.statePre.at<float>(3) = 0;
	setIdentity(Filter.measurementMatrix);
	setIdentity(Filter.processNoiseCov, Scalar::all(1e-3));        //Kowariancja Q
	setIdentity(Filter.measurementNoiseCov, Scalar::all(1e-2));    //Kowariancja R
	setIdentity(Filter.errorCovPost, Scalar::all(.1));

	return Filter;
}
int main() {

	KalmanFilter leftFilter = generateKalman();
	KalmanFilter rightFilter = generateKalman();


	Mat_<float> estimationPointsRight(2, 1);
	Mat_<float> estimationPointsLeft(2, 1);

	estimationPointsRight(0) = 320;
	estimationPointsRight(1) = 240;

	estimationPointsLeft(0) = 320;
	estimationPointsLeft(1) = 240;


	std::string  names_file = "data/coco.names";
	auto obj_names = objects_names_from_file(names_file);


	if (left_cam.fd == -1 || right_cam.fd == -1) {/*exception*/return -1;}

	set_PTZF(&left_cam);
	set_PTZF(&right_cam);

	calibration_mode();

// set most convenient ptzf after calibration
	left_cam.ptzf.pan -= 25;
	right_cam.ptzf.pan += 25;
	set_PTZF(&left_cam);
	set_PTZF(&right_cam);

	Mat cam_img;
	std::vector<bbox> detections;
	bool left_detected, right_detected;

	std::atomic<bool> run(true);
	std::thread cinThread(ReadCin, std::ref(run));


	int leftWithoutDetection = CALMAN_FRAME_WITHOUT_DETECTION;
	int rightWithoutDetection = CALMAN_FRAME_WITHOUT_DETECTION;

	while (true) {
		leftFilter.predict();
		rightFilter.predict();

		try {
			left_detected = false;
			right_detected = false;
			left_cam.ptzf = get_position(left_cam.fd, left_cam.id);
			right_cam.ptzf = get_position(right_cam.fd, right_cam.id);

			cam_img = cameras.getFrameFromCamera(left_cam.serial_number);
			cvtColor(cam_img, cam_img, cv::COLOR_GRAY2RGB);

			detections = detect_object(cam_img, obj_names);
			bbox detection;
			for (auto &d : detections) {
				if (d.left > 0) {
					// printf("%d %d %d %d %d %d \n", detection.left, detection.top, detection.width, detection.height, detection.center_x, detection.center_y);

					if (d.type == tracked_type) {
						left_detected = true;

						ellipse( cam_img, Point(d.center_x, d.center_y), Size( d.width / 2, d.height / 2), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );

						detection = d;

						estimationPointsLeft(0) = d.center_x;
						estimationPointsLeft(1) = d.center_y;

						leftWithoutDetection = 0;
					}

				}
			}
			leftWithoutDetection++;
			if (left_detected) {
				Mat estimatedLeft = leftFilter.correct(estimationPointsLeft);
				// detection.center_x = estimatedLeft.at<float>(0);
				// detection.center_y = estimatedLeft.at<float>(1);
				circle(cam_img, Point(estimatedLeft.at<float>(0), estimatedLeft.at<float>(1)), 20, Scalar( 255, 0, 255 ),  5, 8, 0);
				left_cam.ptzf = calculatePTZF(cam_img.size().width,  cam_img.size().height, detection, left_cam);
			}
			imshow("Left camera", cam_img);

			cam_img = cameras.getFrameFromCamera(right_cam.serial_number);
			cvtColor(cam_img, cam_img, cv::COLOR_GRAY2RGB);
			detections = detect_object(cam_img, obj_names);

			for (auto &d : detections) {
				if (d.left > 0) {
					// printf("%d %d %d %d %d %d \n", detection.left, detection.top, detection.width, detection.height, detection.center_x, detection.center_y);

					if (d.type == tracked_type) {
						right_detected = true;

						ellipse( cam_img, Point(d.center_x, d.center_y), Size( d.width / 2, d.height / 2), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );

						detection = d;

						estimationPointsRight(0) = detection.center_x;
						estimationPointsRight(1) = detection.center_y;
						rightWithoutDetection = 0;
					}
				} 
			}
			rightWithoutDetection++;

			if (right_detected) {

				Mat estimatedRight = rightFilter.correct(estimationPointsRight);
				// detection.center_x = estimatedRight.at<float>(0);
				// detection.center_y = estimatedRight.at<float>(1);
				circle(cam_img, Point(estimatedRight.at<float>(0), estimatedRight.at<float>(1)), 20, Scalar( 255, 0, 255 ),  5, 8, 0);

				right_cam.ptzf = calculatePTZF(cam_img.size().width,  cam_img.size().height, detection, right_cam);;
			}
			imshow("Right camera", cam_img);

			if (!left_detected) {
				left_cam.ptzf.zoom -= 10;
			}
			if (!right_detected) {
				right_cam.ptzf.zoom -= 10;
			}
			if (left_detected && right_detected) {
				calculateCordinates();
			}
			if(left_detected){
				set_PTZF(&left_cam);
			}
			if (right_detected)
			{
				set_PTZF(&right_cam);
			}
		}
		catch (GenICam::GenericException &e) {
			cerr << "An exception occurred.\n" << e.GetDescription() << endl;
		}

		moveWindow("Left camera", 0, 0);
		moveWindow("Right camera", 1200, 0);
		waitKey(1);
	}

	run.store(false);
	cinThread.join();

	return 0;
}
