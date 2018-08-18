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
	float size = 2.54;
	float deltaPan = ((float)atan2((center_x - (img_width / 2)) * size / img_width, getFocal(zoom))) * 180.0 / 3.14;
	//if(deltaPan <0.1) return 0;
	return deltaPan;
}

float getDeltaTilt(int img_height, int center_y, int zoom) {
	float size = 2.54;
	float deltaTilt = ((float)atan2((center_y - (img_height / 2)) * size / img_height, getFocal(zoom))) * 180.0 / 3.14;
	//if(deltaTilt< 0.1) return 0;
	return  deltaTilt;
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

	//printf("\rx: %3.2f | y: %3.2f | left cam dist sqrt^2 | %3.2f, right cam dist sqrt^2: %3.2f", points[0], points[1], points[2], points[3]);

	left_cam.distance_to_object = points[3];
	right_cam.distance_to_object = points[4];
}

int calculateZoom(int img_width, bbox detection, device cam) {
	float lower_limit, upper_limit;
	if (cam.id == left_cam.id) {
		lower_limit = 0.5;
		upper_limit = 0.9;
	}
	if (cam.id == right_cam.id) {
		lower_limit = 0.5;
		upper_limit = 0.9;
	}
	return 250;
	if (detection.width / img_width < lower_limit && cam.ptzf.zoom < 200 ) return cam.ptzf.zoom + 10;
	if (detection.width / img_width < lower_limit && cam.ptzf.zoom < 230 ) return cam.ptzf.zoom + 5;
	if (detection.width / img_width < lower_limit && cam.ptzf.zoom < 250 ) return cam.ptzf.zoom + 2;
	if (detection.width / img_width > upper_limit) {
		return cam.ptzf.zoom - 1;
		}
	}

int calculateFocus(device cam) {
	float dist = cam.distance_to_object;
	if (dist < 1.5) return 0;
	else if (dist > 9) return 190;
	else return (int)(-3.0 * dist * dist + 72 * dist - 75);
}

PTZF calculatePTZF(int img_width, int img_height, bbox detection, device cam) {
	PTZF ptzf;
	ptzf.pan = cam.ptzf.pan + getDeltaPan(img_width, detection.center_x, cam.ptzf.zoom);
	ptzf.tilt = cam.ptzf.tilt + getDeltaTilt(img_height, detection.center_y, cam.ptzf.zoom);
	ptzf.zoom = calculateZoom(img_width, detection, cam);
	if(ptzf.zoom>250)ptzf.zoom = 250;
	ptzf.focus = calculateFocus(cam);
	printf("\n pan: %3.2f | tilt: %3.2f | zoom | %3d, focus: %3d ", ptzf.pan, ptzf.tilt, ptzf.zoom, ptzf.focus);
	return ptzf;
}
