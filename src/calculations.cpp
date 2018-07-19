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

float getDeltaPan(int img_width, int center_x, int zoom) {
	float size = 2.65;
	return ((float)atan2((center_x - (img_width / 2)) * size / img_width, getFocal(zoom))) * 180.0 / 3.14;
}

float getDeltaTilt(int img_height, int center_y, int zoom) {
	float size = 2.65;
	return ((float)atan2((center_y - (img_height / 2)) * size / img_height, getFocal(zoom))) * 180.0 / 3.14;
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

	printf("\rx: %.2f, y: %.2f, left cam dist sqrt^2: %.2f, right cam dist sqrt^2: %.2f", points[0], points[1], points[2], points[3]);

	left_cam.distance_to_object = points[3];
	right_cam.distance_to_object = points[4];
}

int calculateZoom(int img_width, bbox detection, device cam) {
	float lower_limit, upper_limit;
	if (cam.id == left_cam.id) {
		lower_limit = 0.3;
		upper_limit = 0.4;
	}
	if (cam.id == right_cam.id) {
		lower_limit = 0.4;
		upper_limit = 0.5;
	}
	if (detection.w / img_width < lower_limit && cam.ptzf.zoom < 200 ) return cam.ptzf.zoom + 10;
	else if (detection.w / img_width > upper_limit) return cam.ptzf.zoom - 10;
}

int calculateFocus(device cam) {
	float dist = cam.distance_to_object;
	if (dist < 1.5) return 0;
	else if (dist > 9) return 190;
	else return (int)(-3.5 * dist * dist + 59.2 * dist - 69);
}

PTZF calculatePTZF(int img_width, int img_height, bbox detection, device cam) {
	PTZF ptzf;
	ptzf.pan = cam.ptzf.pan + getDeltaPan(img_width, detection.center_x, cam.ptzf.zoom);
	ptzf.tilt = cam.ptzf.tilt + getDeltaTilt(img_height, detection.center_y, cam.ptzf.zoom);
	ptzf.zoom = calculateZoom(img_width, detection, cam);
	ptzf.focus = calculateFocus(cam);
	return ptzf;
}
