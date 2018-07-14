#ifndef STRUCTS_H
#define STRUCTS_H

struct position {
	float x, y, z;
};

struct PTZF {
	float pan, tilt, zoom, focus;
};

struct bbox {
	int x, y, w, h;
};

struct device {
	char name[15];
	int id;
	int fd;
	char serial_number[9];
};

#endif /* STRUCTS_H */