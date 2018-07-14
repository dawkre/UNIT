#include "cameras.h"

  Cameras::Cameras() : camerasArray(numberOfCameras) {
    fc.OutputPixelFormat = PixelType_Mono8;

    CTlFactory& tlFactory = CTlFactory::GetInstance();
    DeviceInfoList_t devices;
    tlFactory.EnumerateDevices(devices);

    camerasArray[ 0 ].Attach( tlFactory.CreateDevice( devices[ 0 ]));
	model[0] = camerasArray[0].GetDeviceInfo().GetSerialNumber();

    camerasArray[ 1 ].Attach( tlFactory.CreateDevice( devices[ 1 ]));
	model[1] = camerasArray[1].GetDeviceInfo().GetSerialNumber();

    camerasArray.StartGrabbing();
  }

  Mat Cameras::getFrameFromCamera(string camera_serial_number) {
  	int cameraIndex;
  	if(camera_serial_number == model[0]) cameraIndex = 0;
  	if(camera_serial_number == model[1]) cameraIndex = 1;
    camerasArray[cameraIndex].RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
    fc.Convert(image, ptrGrabResult);
    return cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC1, (uint8_t*)image.GetBuffer());
  }