#include "cameras.h"

  Cameras::Cameras() : camerasArray(numberOfCameras) {
    fc.OutputPixelFormat = PixelType_Mono8;

    CTlFactory& tlFactory = CTlFactory::GetInstance();
    DeviceInfoList_t devices;
    tlFactory.EnumerateDevices(devices);

    camerasArray[ 0 ].Attach( tlFactory.CreateDevice( devices[ 0 ]));
    camerasArray[ 1 ].Attach( tlFactory.CreateDevice( devices[ 1 ]));
    camerasArray.StartGrabbing();
  }

  Mat Cameras::getFrameFromCamera(int cameraIndex) {
    camerasArray[cameraIndex].RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
    fc.Convert(image, ptrGrabResult);
    return cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC1, (uint8_t*)image.GetBuffer());
  }