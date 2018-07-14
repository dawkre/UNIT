#include "detection.h"
#include <stdio.h>

bbox detect_object( Mat frame, CascadeClassifier face_cascade )
{
	std::vector<Rect> faces;

	equalizeHist( frame, frame );

	face_cascade.detectMultiScale( frame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30) );
	bbox detection ;
	if (  faces.size() > 0)
	{
		detection = {
			faces[0].x,
			faces[0].y,
			faces[0].width,
			faces[0].height
		};

	} else {
		detection = {0, 0, 0, 0};
	}

	return detection;
}
