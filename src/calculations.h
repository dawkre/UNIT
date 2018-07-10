struct position {
	float x, y, z;
};

struct PTZF {
	int pan, tilt, zoom, focus;
};

position get_position(/*bbox left_cam_detecion, bbox right_cam_detection*/);
PTZF calculate_PTZF(/*position object_coordinates*/);