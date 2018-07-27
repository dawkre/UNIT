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
device left_cam {"/dev/ttyMXUSB0", 0, initializeDevice((char*)"/dev/ttyMXUSB0"), "21818297",  {157,90,0,0}, {0, 0, 0, 0}};
device right_cam {"/dev/ttyMXUSB1", 1,	initializeDevice((char*)"/dev/ttyMXUSB1"), "21855432", {206,90,0,0} , {0, 0, 0, 0}};

Cameras cameras;
int main() {
	Ptr<cuda::CascadeClassifier> face_cascade = cuda::CascadeClassifier::create(
	            "../haarcascades_cuda_4.0.0/haarcascade_frontalface_alt.xml");

	if (left_cam.fd == -1 || right_cam.fd == -1) {/*exception*/return -1;}
	set_PTZF(&left_cam);
	set_PTZF(&right_cam);

	calibration_mode();

	Mat cam_img;
	bbox detection;
	bool left_detected, right_detected;

	while (true) {
			double time = what_time_is_it_now();

		try {
			left_cam.ptzf = get_position(left_cam.fd, left_cam.id);
			right_cam.ptzf = get_position(right_cam.fd, right_cam.id);

			cam_img = cameras.getFrameFromCamera(left_cam.serial_number);
			detection = detect_object(cam_img, face_cascade);
			bbox d = detection;
			// printf("%d %d %d %d %d %d \n", d.left, d.top, d.width, d.height, d.center_x, d.center_y);
			left_detected = false;
			right_detected = false;

			if (detection.left > 0) {
				left_detected = true;
				ellipse( cam_img, Point(detection.center_x, detection.center_y), Size( detection.width / 2, detection.height / 2), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );
				left_cam.ptzf = calculatePTZF(cam_img.size().width,  cam_img.size().height, detection, left_cam);
			}

			imshow("Left camera", cam_img);

			cam_img = cameras.getFrameFromCamera(right_cam.serial_number);
			detection = detect_object(cam_img, face_cascade);

			if (detection.left > 0) {
				right_detected = true;
				ellipse( cam_img, Point(detection.center_x, detection.center_y), Size( detection.width / 2, detection.height / 2), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );
				right_cam.ptzf = calculatePTZF(cam_img.size().width,  cam_img.size().height, detection, right_cam);;
			}

			imshow("Right camera", cam_img);

			if (!left_detected) {
				left_cam.ptzf.zoom -= 20;
			}
			if (!right_detected) {
				right_cam.ptzf.zoom -= 20;
			}
			if (left_detected && right_detected) {
				calculateCordinates();
			}

			set_PTZF(&left_cam);
			set_PTZF(&right_cam);
		}
		catch (GenICam::GenericException &e) {
			cerr << "An exception occurred.\n" << e.GetDescription() << endl;
		}

		moveWindow("Left camera", 0, 0);
		moveWindow("Right camera", 640, 0);
		waitKey(1);
				printf("%f\n", what_time_is_it_now()-time);

	}
	return 0;
}
