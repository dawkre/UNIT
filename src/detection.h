/*
The detection files contain all detection-related logic.
The detect_object method should return a bbox object that describes
the bounding box around the detected object.
*/

#include "structs.h"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

bbox detect_object( Mat frame, CascadeClassifier face_cascade );
CascadeClassifier initialize_detector(String cascade_name);