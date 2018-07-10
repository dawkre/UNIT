#include <pylon/PylonIncludes.h>
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace Pylon;
using namespace std;

class Cameras {
	static const size_t numberOfCameras = 2;
	PylonAutoInitTerm autoInitTerm;
	CInstantCameraArray camerasArray;
	DeviceInfoList_t devices;
	CGrabResultPtr ptrGrabResult;
	CPylonImage image;
	CImageFormatConverter fc;

public:
	Cameras();
	Mat getFrameFromCamera(int cameraIndex) ;
};