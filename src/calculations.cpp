#include "calculations.h"
#include "math.h"

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
