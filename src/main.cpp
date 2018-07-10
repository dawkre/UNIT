#include "detection.h"
#include "cameras.h"
#include "calculations.h"

extern "C" {
#include "PTZF_control.h"
}

#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

int main() {
	/*PTZF control initialization*/
	char dev0[] = "/dev/ttyMXUSB0";
	char dev1[] = "/dev/ttyMXUSB1";
	int dev0Id = 0;
	int dev1Id = 1;
	int fd0 = initializeDevice(dev0);
	int fd1 = initializeDevice(dev1);
	
	position object_coordinates = get_position();

	printf("\ncalculations module test:\n");
	printf("position: x:%.2f, y:%.2f, z:%.2f\n", object_coordinates.x, object_coordinates.y, object_coordinates.z);
	PTZF ptzf = calculate_PTZF();
	printf("ptzf: pan:%d, tilt:%d, zoom:%d, focus:%d\n", ptzf.pan, ptzf.tilt, ptzf.zoom, ptzf.focus);

	Cameras cameras;
	Mat cam_img;

	while (true) {
		try {
			cam_img = cameras.getFrameFromCamera(0);
			imshow("Left camera", cam_img);
			cam_img = cameras.getFrameFromCamera(1);
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