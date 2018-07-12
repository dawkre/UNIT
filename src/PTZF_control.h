/*
The PTZF_control file contains all the methods needed for the control of the PTZF parameters
(that is pan, tilt, zoom and focus). To initialize communication with the hardware,
the initializeDevice method should be used. It returns file descriptor (fd) that is
later needed by other PTZF_control functions.
*/

int initializeDevice(char *deviceName);
int oblicz_crc(int* bufor, int ilosc);
char getch();
void set_control(int fd, int id_device);
void set_pan(int fd, int id_device, int angle);
void set_tilt(int fd, int id_device, int angle);
void set_PTZF(int fd, int id_device, float pan, float tilt, int speed_pan, int speed_tilt, int zoom, int focus);
void set_zoom(int fd, int id_device, int position);
void set_focus(int fd, int id_device, int position);
void speed_pan(int fd, int id_device, int speed);
void speed_tilt(int fd, int id_device, int speed);
void speed_zoom(int fd, int id_device, int speed);
void speed_focus_iris(int fd, int id_device, int speed);
void get_position(int fd, int id_device, float pan, float tilt, int zoom, int focus);