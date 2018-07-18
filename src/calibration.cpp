#include "calibration.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "globals.h"

void calibration_mode() {
	printf("\nCalibration mode.\n"
	       "arrow keys-\tpan and tilt\n");
	printf("Calibrating LEFT camera. Press Enter key when ready.\n");
	left_cam.ptzf = loop(left_cam);
	left_cam.offset = left_cam.ptzf;
	printf("Calibrating RIGHT camera. Press Enter key when ready.\n");
	right_cam.ptzf = loop(right_cam);
	right_cam.offset = right_cam.ptzf;
}

PTZF loop(device cam) {
	Mat cam_img;
	char input = 'a';

	while (input != '\n') {
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
		}
		set_PTZF(&cam);
	}
	printf("Offset set: {%.0f,%.0f,%d,%d} \n", cam.ptzf.pan, cam.ptzf.tilt, cam.ptzf.zoom, cam.ptzf.focus);
	return cam.ptzf;
}
