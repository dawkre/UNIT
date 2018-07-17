/*
The Cameras class is a class allowing to capture the image from the Basler cameras used in the project.
When the class is instantiated, the communication with the cameras is initialized.
The frames from the cameras can then be captured using the getFrameFromCamera method.
This code draws from the Grab_MultipleCameras code avilable in Pylon's examples.
*/
#ifndef CAMERAS_H
#define CAMERAS_H

#include <pylon/PylonIncludes.h>
#include "opencv2/core/core.hpp"
#include <string.h>

using namespace cv;
using namespace Pylon;
using namespace std;

class Cameras {
	string model[2];
	static const size_t numberOfCameras = 2;
	PylonAutoInitTerm autoInitTerm;
	CInstantCameraArray camerasArray;
	DeviceInfoList_t devices;
	CGrabResultPtr ptrGrabResult;
	CPylonImage image;
	CImageFormatConverter fc;

public:
	Cameras();
	Mat getFrameFromCamera(string camera_serial_number);
};

#endif /* CAMERAS_H */