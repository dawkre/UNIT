#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
	float pan, tilt;
	int zoom, focus;
} PTZF;

typedef struct  {
	int left, top, width, height, center_x, center_y;
} bbox;

typedef struct  {
	char name[15];
	int id;
	int fd;
	char serial_number[9];
	PTZF ptzf;
	PTZF offset;
	float distance_to_object;
} device;

#endif /* STRUCTS_H */
