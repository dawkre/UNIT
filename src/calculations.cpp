#include "calculations.h"
#include "math.h"
#include "globals.h"

float getFocal(int zoom)
{
	if (zoom <= 200) {
		return 2 * pow(10, -6) * pow(zoom, 3) - 0.0003 * pow(zoom, 2) + 0.0446 * zoom + 8.7609;
	}
	else {
		zoom -= 200;
		return 0.0012 * pow(zoom, 3) - 0.0688 * pow(zoom, 2) + 1.1299 * zoom + 24.487;
	}
}

float getDeltaPan(int imageWidth, int positon, int zoom) {
	float size = 2.65;
	return ((float)atan2((positon - (imageWidth / 2)) * size / imageWidth, getFocal(zoom))) * 180.0 / 3.14;
}

float getDeltaTilt(int imageWidth, int positon, int zoom) {
	float size = 2.65;
	return ((float)atan2((positon - (imageWidth / 2)) * size / imageWidth, getFocal(zoom))) * 180.0 / 3.14;
}

void calculateCordinates() {
	float pan1 = left_cam.ptzf.pan - left_cam.offset.pan;
	float pan2 = right_cam.ptzf.pan - right_cam.offset.pan;
	float tilt1 = left_cam.ptzf.tilt - left_cam.offset.tilt;
	float tilt2 = right_cam.ptzf.tilt - right_cam.offset.tilt;
	float PI = 3.14159265;
	float l = 0.60;
	float points[5];
	points[0] = ((float)tan ( (pan1 + 90) * PI / 180.0 ) * (float)tan ( (90 - pan2) * PI / 180.0 ) * l) / ((float)tan ( (pan1 + 90) * PI / 180.0 ) + (float)tan ( (90 - pan2) * PI / 180.0 ));
	points[1] = points[0] / tan ( (pan1 + 90) * PI / 180.0 );
	points[2] = sqrt(points[0] * points[0] + points[1] * points[1]) * tan ( (-tilt1) * PI / 180.0 );
	points[3] = sqrt(points[0] * points[0] + points[1] * points[1] + points[2] * points[2]);
	points[4] = sqrt(points[0] * points[0] + (points[1] - l) * (points[1] - l) + points[2] * points[2]);

	printf("Odległość x %f \n", points[0]);
	printf("Odległość y %f \n", points[1]);

	printf("Odległość sqrt^2 %f \n", points[3]);
	printf("Odległość2 sqrt^2 %f \n", points[4]);
}

int calculateZoom(int img_width, bbox detection, device cam) {
	float lower_limit, upper_limit;
	if (cam.id == left_cam.id) {
		lower_limit = 0.4;
		upper_limit = 0.5;
	}
	if (cam.id == right_cam.id) {
		lower_limit = 0.5;
		upper_limit = 0.6;
	}
	if (detection.w / img_width < lower_limit && cam.ptzf.zoom < 200 ) return cam.ptzf.zoom+10;
	else if (detection.w / img_width > upper_limit) return cam.ptzf.zoom-10;
}

// void calculateFocus() {
// 	if (points[4] < 1.5)focus2 = 0;
// 	else if (points[4] > 9) focus2 = 190;
// 	else focus2 = -3.5 * points[4] * points[4] + 59.2 * points[4] - 69;
// 	printf("Focus2 %i \n", focus2);

// 	if (points[3] < 1.5)focus1 = 0;
// 	else if (points[3] > 9) focus1 = 190;
// 	else focus1 = -3.5 * points[3] * points[3] + 59.2 * points[3] - 69;
// 	printf("Focus1 %i \n", focus1);
// }
