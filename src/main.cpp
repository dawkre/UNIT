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
	/*Initialize detector*/
	CascadeClassifier face_cascade = initialize_detector("../cascades/haarcascade_frontalface_alt.xml");

	/*Initialize devices*/
	device left_cam {"/dev/ttyMXUSB0", 0, initializeDevice((char*)"/dev/ttyMXUSB0"), "21818297", {100,70,0,0}};
	device right_cam {"/dev/ttyMXUSB1", 1, initializeDevice((char*)"/dev/ttyMXUSB1"), "21855432", {190,70,0,0}};
	if (left_cam.fd == -1 || right_cam.fd == -1) {/*exception*/return -1;}
	set_PTZF(left_cam);
	set_PTZF(right_cam);

	Cameras cameras;
	Mat cam_img;
	bbox detection;
	PTZF ptzf;

	while (true) {
		try {
			left_cam.ptzf = get_position(left_cam.fd, left_cam.id);
			right_cam.ptzf = get_position(right_cam.fd, right_cam.id);

			cam_img = cameras.getFrameFromCamera(left_cam.serial_number);
			detection = detect_object(cam_img, face_cascade);

			if (detection.x > 0) {
	 			ptzf = left_cam.ptzf;

				Point center( detection.x + detection.w / 2, detection.y + detection.h / 2 );
				ellipse( cam_img, center, Size( detection.w / 2, detection.h / 2), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );

				ptzf.pan = ptzf.pan + getDeltaPan( cam_img.size().width, center.x, ptzf.zoom);
				ptzf.tilt = ptzf.tilt + getDeltaTilt( cam_img.size().height, center.y ,  ptzf.zoom);

				left_cam.ptzf = ptzf;
			}
			imshow("Left camera", cam_img);

			cam_img = cameras.getFrameFromCamera(right_cam.serial_number);
			detection = detect_object(cam_img, face_cascade);

			if (detection.x > 0) {
				ptzf = right_cam.ptzf;

				Point center( detection.x + detection.w / 2, detection.y + detection.h / 2 );
				ellipse( cam_img, center, Size( detection.w / 2, detection.h / 2), 0, 0, 360, Scalar( 255, 0, 255 ), 2, 8, 0 );

				ptzf.pan = ptzf.pan + getDeltaPan( cam_img.size().width, center.x, ptzf.zoom);
				ptzf.tilt = ptzf.tilt + getDeltaTilt( cam_img.size().height, center.y ,  ptzf.zoom);

				right_cam.ptzf = ptzf;
			}
			imshow("Right camera", cam_img);

			set_PTZF(left_cam);
			set_PTZF(right_cam);
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
