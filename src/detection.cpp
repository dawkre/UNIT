#include "detection.h"
#include <stdio.h>
#include <sys/time.h>

double what_time_is_it_now()
{
	struct timeval time;
	if (gettimeofday(&time, NULL)) {
		return 0;
	}
	return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
bbox detect_object( Mat frame, Ptr<cuda::CascadeClassifier> cascade )
{
	cuda::GpuMat frame_gpu(frame);
	cuda::GpuMat objectsBuf_gpu;
	double time;

	time = what_time_is_it_now();
	cascade->detectMultiScale( frame_gpu, objectsBuf_gpu);
	time = what_time_is_it_now() - time;
	// printf("%f\n", time);

	std::vector<Rect> faces;
	cascade->convert(objectsBuf_gpu, faces);
	Rect face(-1, -1, -1, -1);
	for (int i = 0; i < faces.size(); ++i) {
		if (faces[i].width > face.width) face = faces[i];
	}

	bbox detection = {
		face.x,
		face.y,
		face.width,
		face.height,
		face.x + face.width / 2,
		face.y + face.height / 2
	};

	return detection;
}

// gpu::CascadeClassifier_GPU initialize_detector(String cascade_name) {
// 	gpu::CascadeClassifier_GPU cascade;
// 	if ( !cascade.load( cascade_name ) ) { printf("detector initialization failed\n"); throw - 1; };
// 	return cascade;
// }
