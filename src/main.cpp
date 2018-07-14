#include "detection.h"
#include "cameras.h"
#include "calculations.h"
#include "structs.h"

extern "C" {
#include "PTZF_control.h"
}

#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

int main() {
	CascadeClassifier face_cascade;
	String face_cascade_name = "../cascades/haarcascade_frontalface_alt.xml";
	if ( !face_cascade.load( face_cascade_name ) ) { printf("--(!)Error loading\n"); return -1; };

	device left_cam {"/dev/ttyMXUSB0", 0, -1, "21818297"};
	device right_cam {"/dev/ttyMXUSB1", 1, -1, "21855432"};
	left_cam.fd = initializeDevice(left_cam.name);
	right_cam.fd = initializeDevice(right_cam.name);
	if(left_cam.fd==-1 || right_cam.fd==-1){/*exception*/return -1;}
	
	Cameras cameras;
	Mat cam_img;

	while (true) {
		try {
			cam_img = cameras.getFrameFromCamera(left_cam.serial_number);
			bbox detection = detect_object(cam_img, face_cascade);

			if (detection.x > 0) {
				Point center( detection.x + detection.w / 2, detection.y + detection.h / 2 );
				ellipse( cam_img, center, Size( detection.w / 2, detection.h / 2), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );
			}
			imshow("Left camera", cam_img);

			cam_img = cameras.getFrameFromCamera(right_cam.serial_number);
			detection = detect_object(cam_img, face_cascade);

			if (detection.x > 0) {
				Point center( detection.x + detection.w / 2, detection.y + detection.h / 2 );
				ellipse( cam_img, center, Size( detection.w / 2, detection.h / 2), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );
			}
			imshow("Right camera", cam_img);
		}
		catch (GenICam::GenericException &e) {
			cerr << "An exception occurred.\n" << e.GetDescription() << endl;
		}

		moveWindow("Left camera", 0, 0);
		moveWindow("Right camera", 640, 0);
		waitKey(1);
	}
	return 0;
}