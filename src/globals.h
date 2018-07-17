#ifndef GLOBALS_H
#define GLOBALS_H

#include "cameras.h"
extern "C" {
#include "PTZF_control.h"
}

extern device left_cam;

extern device right_cam;

extern Cameras cameras;

#endif /* GLOBALS_H */