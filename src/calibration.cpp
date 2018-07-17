#include "calibration.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "globals.h"

void calibration_mode() {
	printf("Calibrating left camera. Use arrow to pan and tilt, +/- to zoom in/out and f/F to change focus.\n");
	left_cam.ptzf = loop(left_cam);
	left_cam.offset = left_cam.ptzf;
	printf("Calibrating right camera. Use arrow to pan and tilt, +/- to zoom in/out and f/F to change focus.\n");
	right_cam.ptzf = loop(right_cam);
	right_cam.offset = right_cam.ptzf;
}

PTZF loop(device cam) {
	Mat cam_img;
	char input = 'a';
	int i = 5;
	while (input != '\n') {
		if (i % 5 == 0) {
			cam_img = cameras.getFrameFromCamera(cam.serial_number);
			imshow("Calibrating camera", cam_img);
			waitKey(1);
			i = 1;
		}
		i++;
		input = getch();

		switch (input) {
		case 65:
			cam.ptzf.tilt--;
			break;
		case 66:
			cam.ptzf.tilt++;
			break;
		case 67:
			cam.ptzf.pan++;
			break;
		case 68:
			cam.ptzf.pan--;
			break;
		case '+':
			cam.ptzf.zoom++;
			break;
		case '-':
			cam.ptzf.zoom--;
			break;
		case 'F':
			cam.ptzf.focus++;
			break;
		case 'f':
			cam.ptzf.focus--;
			break;
		}
		set_PTZF(cam);
	}
	return cam.ptzf;
}