#include "detection.h"
#include <stdio.h>

bbox detect_object( Mat frame, CascadeClassifier cascade )
{
	std::vector<Rect> objects;

	equalizeHist( frame, frame );

	cascade.detectMultiScale( frame, objects, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30) );
	bbox detection = { -1, -1, -1, -1, -1, -1};
	if (  objects.size() > 0)
	{
		detection = {
			objects[0].x,
			objects[0].y,
			objects[0].width,
			objects[0].height,
			objects[0].x + objects[0].width / 2,
			objects[0].y + objects[0].height / 2
		};
	}

	return detection;
}

CascadeClassifier initialize_detector(String cascade_name) {
	CascadeClassifier cascade;
	if ( !cascade.load( cascade_name ) ) { printf("detector initialization failed\n"); throw - 1; };
	return cascade;
}

// bbox detect_object( Mat frame, gpu::CascadeClassifier_GPU cascade )
// {
// 	gpu::GpuMat frame_gpu(frame);
// 	gpu::GpuMat objectsBuf_gpu;
// 	Mat objects_downloaded;
// 	int detections_num = cascade.detectMultiScale( frame_gpu, objectsBuf_gpu, 1.2, 2, Size(frame.cols/4, frame.rows/4) );
// 	bbox detection = { -1, -1, -1, -1};
// 	if ( detections_num > 0)
// 	{
// 		objectsBuf_gpu.colRange(0,1).download(objects_downloaded);
// 		Rect * rects = objects_downloaded.ptr<Rect>();
// 		detection = {
// 			rects[0].x,
// 			rects[0].y,
// 			rects[0].width,
// 			rects[0].height
// 		};
// 	}

// 	return detection;
// }

// gpu::CascadeClassifier_GPU initialize_detector(String cascade_name) {
// 	gpu::CascadeClassifier_GPU cascade;
// 	if ( !cascade.load( cascade_name ) ) { printf("detector initialization failed\n"); throw - 1; };
// 	return cascade;
// }
