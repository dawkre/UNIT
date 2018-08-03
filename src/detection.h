#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/core/types_c.h"
#include "opencv2/core/version.hpp"
using namespace cv;
#include "yolo_v2_class.hpp"    // imported functions from DLL
#include "structs.h"

std::vector<bbox> detect_object(Mat frame, std::vector<std::string> obj_names);
std::vector<std::string> objects_names_from_file(std::string const filename);
