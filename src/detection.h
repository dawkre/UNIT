/*
The detection files contain all detection-related logic.
The detect_object method should return a bbox object that describes
the bounding box around the detected object.
*/

#include "structs.h"
#include "opencv2/core/core.hpp"
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/core/gpumat.hpp"
using namespace cv;
double what_time_is_it_now();
bbox detect_object( Mat frame, Ptr<cuda::CascadeClassifier> cascade );
// gpu::CascadeClassifier_GPU initialize_detector(String cascade_name);
