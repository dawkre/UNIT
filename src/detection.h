#include "opencv2/core/core.hpp"
using namespace cv;

struct bbox {
	int x, y, w, h;
};

bbox detect_object(Mat image);