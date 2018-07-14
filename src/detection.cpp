#include "detection.h"
#include <stdio.h>

bbox detect_object( Mat frame, CascadeClassifier cascade )
{
	std::vector<Rect> objects;

	equalizeHist( frame, frame );

	cascade.detectMultiScale( frame, objects, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30) );
	bbox detection = {-1,-1,-1,-1};
	if (  objects.size() > 0)
	{
		detection = {
			objects[0].x,
			objects[0].y,
			objects[0].width,
			objects[0].height
		};
	}

	return detection;
}

CascadeClassifier initialize_detector(String cascade_name) {
	CascadeClassifier cascade;
	if ( !cascade.load( cascade_name ) ) { printf("detector initialization failed\n"); throw - 1; };
	return cascade;
}