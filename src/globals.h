#ifndef GLOBALS_H
#define GLOBALS_H

#include "cameras.h"
extern "C" {
#include "PTZF_control.h"
}

extern device left_cam;

extern device right_cam;

extern Cameras cameras;

#include "yolo_v2_class.hpp"    // imported functions from DLL

extern std::string cfg_file;
extern std::string weights_file;
extern Detector detector;

#endif /* GLOBALS_H */
