/*
The PTZF_control file contains all the methods needed for the control of the PTZF parameters
(that is pan, tilt, zoom and focus). To initialize communication with the hardware,
the initializeDevice method should be used. It returns file descriptor (fd) that is
later needed by other PTZF_control functions.
*/
#include "structs.h"
#include <stdint.h>
int initializeDevice(char *deviceName);
uint16_t oblicz_crc(uint8_t* bufor, int ilosc);
char getch();
void set_control(int fd, int id_device);
void set_pan(int fd, int id_device, int angle);
void set_tilt(int fd, int id_device, int angle);
void set_zoom(int fd, int id_device, int position);
void set_focus(int fd, int id_device, int position);
void speed_pan(int fd, int id_device, int speed);
void speed_tilt(int fd, int id_device, int speed);
void speed_zoom(int fd, int id_device, int speed);
void speed_focus_iris(int fd, int id_device, int speed);
void speed_PTZ(int fd, uint8_t id_device, int speed_pan, int speed_tilt, int speed_zoom);
PTZF get_position(int fd, int id_device);
void set_PTZF(device* dev);