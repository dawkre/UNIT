#include "structs.h"

float getFocal(int zoom);
float getDeltaPan(int imageWidth, int positon, int zoom);
float getDeltaTilt(int imageWidth, int positon, int zoom);
void calculateCordinates();
int calculateZoom(int img_width, bbox detection, device cam);
int calculateFocus(device cam);
