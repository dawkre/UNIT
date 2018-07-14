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
	device left_cam {"/dev/ttyMXUSB0", 0, -1, "21818297"};
	device right_cam {"/dev/ttyMXUSB1", 1, -1, "21855432"};
	left_cam.fd = initializeDevice(left_cam.name);
	right_cam.fd = initializeDevice(right_cam.name);
	if(left_cam.fd==-1 || right_cam.fd==-1){/*exception*/return -1;}
	
	position object_coordinates = get_position();

	printf("\ncalculations module test:\n");
	printf("position: x:%.2f, y:%.2f, z:%.2f\n", object_coordinates.x, object_coordinates.y, object_coordinates.z);
	PTZF ptzf = calculate_PTZF();
	printf("ptzf: pan:%f, tilt:%f, zoom:%f, focus:%f\n", ptzf.pan, ptzf.tilt, ptzf.zoom, ptzf.focus);

	Cameras cameras;
	Mat cam_img;

	while (true) {
		try {
			cam_img = cameras.getFrameFromCamera(left_cam.serial_number);
			imshow("Left camera", cam_img);
			cam_img = cameras.getFrameFromCamera(right_cam.serial_number);
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