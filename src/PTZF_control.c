#include "PTZF_control.h"

#include <stdio.h>
#include <string.h>
// #include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#include <sys/ioctl.h>
#include <asm/ioctls.h>
#include <linux/serial.h>

int initializeDevice(char *deviceName) {
	int fd = open(deviceName, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
		printf("%s: Unable to open. Check if device is connected and program is run as root.\n", deviceName);
		return -1;
	}
	return fd;
}

uint16_t oblicz_crc(uint8_t* bufor, int ilosc)
{
	uint16_t crc = 0xffff;
	uint8_t i, j;
	for (i = 0; i < ilosc; i++)
	{
		crc ^= *(bufor + i);
		for (j = 0; j < 8; j++)
		{
			if (crc & 0x0001)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
				crc >>= 1;
		}
	}
	return (crc);
}

char getch() {
	char buf = 0;
	struct termios old = {0};
	fflush(stdout);
	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
		perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror ("tcsetattr ~ICANON");
	// printf("%c\n",buf);
	return buf;
}

void set_control(int fd, int id_device)
{
	uint8_t buf[] = {id_device, 0x04, 0x51, 0x80, 0x80, 0x80, 0x10};
	uint8_t ile = sizeof(buf) / sizeof(uint8_t);
	uint16_t wynik = oblicz_crc(buf, ile);

	uint8_t array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x04, 0x51, 0x80, 0x80, 0x80, 0x10, array[1], array[0]};
	/* Send content to RS485 */
	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

void set_pan(int fd, int id_device, int angle)
{
	int position = (int)(50 * angle);
	char hex[] = "";
	char hex_low[4], hex_high[4];

	sprintf(hex, "%04X", position);
	hex_high[0] = '0';
	hex_high[1] = 'x';
	hex_high[2] = hex[0];
	hex_high[3] = hex[1];
	hex_low[0] = '0';
	hex_low[1] = 'x';
	hex_low[2] = hex[2];
	hex_low[3] = hex[3];

	uint8_t uint_low = strtoul(hex_low, NULL, 0);
	uint8_t uint_high = strtoul(hex_high, NULL, 0);

	uint8_t buf[] = {id_device, 0x02, 0x31, uint_high, uint_low};
	uint8_t ile = sizeof(buf) / sizeof(uint8_t);
	uint16_t wynik = oblicz_crc(buf, ile);

	uint8_t array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x02, 0x31, uint_high, uint_low, array[1], array[0]};

	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

void set_tilt(int fd, int id_device, int angle)
{
	int position = (int)(50 * angle);
	char hex[] = "";
	char hex_low[4], hex_high[4];

	sprintf(hex, "%04X", position);
	hex_high[0] = '0';
	hex_high[1] = 'x';
	hex_high[2] = hex[0];
	hex_high[3] = hex[1];
	hex_low[0] = '0';
	hex_low[1] = 'x';
	hex_low[2] = hex[2];
	hex_low[3] = hex[3];

	uint8_t uint_low = strtoul(hex_low, NULL, 0);
	uint8_t uint_high = strtoul(hex_high, NULL, 0);

	uint8_t buf[] = {id_device, 0x02, 0x32, uint_high, uint_low};
	uint8_t ile = sizeof(buf) / sizeof(uint8_t);
	uint16_t wynik = oblicz_crc(buf, ile);

	uint8_t array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x02, 0x32, uint_high, uint_low, array[1], array[0]};

	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

void set_zoom(int fd, int id_device, int position)
{
	char hex_full[] = "";

	sprintf(hex_full, "0x%02X", position);

	uint8_t uint_zoom = strtoul(hex_full, NULL, 0);
	uint8_t buf[] = {id_device, 0x01, 0x33, uint_zoom};
	uint8_t ile = sizeof(buf) / sizeof(uint8_t);
	uint16_t wynik = oblicz_crc(buf, ile);

	uint8_t array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x01, 0x33, uint_zoom, array[1], array[0]};

	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

void set_focus(int fd, int id_device, int position)
{
	char hex_full[] = "";

	sprintf(hex_full, "0x%02X", position);

	uint8_t uint_focus = strtoul(hex_full, NULL, 0);
	uint8_t buf[] = {id_device, 0x01, 0x34, uint_focus};
	uint8_t ile = sizeof(buf) / sizeof(uint8_t);
	uint16_t wynik = oblicz_crc(buf, ile);

	uint8_t array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x01, 0x34, uint_focus, array[1], array[0]};

	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

void speed_pan(int fd, int id_device, int speed)
{
	char hex_full[] = "";

	sprintf(hex_full, "0x%02X", speed);

	uint8_t uint_speed = strtoul(hex_full, NULL, 0);
	uint8_t buf[] = {id_device, 0x04, 0x51, uint_speed, 0x80, 0x80, 0x10};
	uint8_t ile = sizeof(buf) / sizeof(uint8_t);
	uint16_t wynik = oblicz_crc(buf, ile);

	uint8_t array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x04, 0x51, uint_speed, 0x80, 0x80, 0x10, array[1], array[0]};

	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

void speed_tilt(int fd, int id_device, int speed)
{
	char hex_full[] = "";

	sprintf(hex_full, "0x%02X", speed);

	uint8_t uint_speed = strtoul(hex_full, NULL, 0);
	uint8_t buf[] = {id_device, 0x04, 0x51, 0x80, uint_speed, 0x80, 0x10};
	uint8_t ile = sizeof(buf) / sizeof(uint8_t);
	uint16_t wynik = oblicz_crc(buf, ile);

	uint8_t array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x04, 0x51, 0x80, uint_speed, 0x80, 0x10, array[1], array[0]};

	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

void speed_zoom(int fd, int id_device, int speed)
{
	char hex_full[] = "";

	sprintf(hex_full, "0x%02X", speed);

	uint8_t uint_speed = strtoul(hex_full, NULL, 0);
	uint8_t buf[] = {id_device, 0x04, 0x51, 0x80, 0x80, uint_speed, 0x10};
	uint8_t ile = sizeof(buf) / sizeof(uint8_t);
	uint16_t wynik = oblicz_crc(buf, ile);

	uint8_t array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x04, 0x51, 0x80, 0x80, uint_speed, 0x10, array[1], array[0]};

	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

void speed_focus_iris(int fd, int id_device, int speed)
{
	char hex_full[] = "";

	sprintf(hex_full, "0x%02X", speed);

	uint8_t uint_speed = strtoul(hex_full, NULL, 0);
	uint8_t buf[] = {id_device, 0x04, 0x51, 0x80, 0x80, 0x80, uint_speed};
	uint8_t ile = sizeof(buf) / sizeof(uint8_t);
	uint16_t wynik = oblicz_crc(buf, ile);

	uint8_t array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x04, 0x51, 0x80, 0x80, 0x80, uint_speed, array[1], array[0]};
	/* Send content to RS485 */
	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

void speed_PTZ(int fd, uint8_t id_device, int speed_pan, int speed_tilt, int speed_zoom)
{
	char hex_pan[5], hex_tilt[5], hex_zoom[5];

	sprintf(hex_pan, "0x%02X", speed_pan);
	sprintf(hex_tilt, "0x%02X", speed_tilt);
	sprintf(hex_zoom, "0x%02X", speed_zoom);

	uint8_t uint_speed_pan = strtoul(hex_pan, NULL, 0);
	uint8_t uint_speed_tilt = strtoul(hex_tilt, NULL, 0);
	uint8_t uint_speed_zoom = strtoul(hex_zoom, NULL, 0);

	uint8_t buf[] = {id_device, 0x04, 0x51, uint_speed_pan, uint_speed_tilt, uint_speed_zoom, 0x10};
	uint8_t ile = sizeof(buf) / sizeof(uint8_t);
	uint16_t wynik = oblicz_crc(buf, ile);

	uint8_t array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x04, 0x51, uint_speed_pan, uint_speed_tilt, uint_speed_zoom, 0x10, array[1], array[0]};

	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

PTZF get_position(int fd, int id_device)
{
	PTZF ptzf;

	uint8_t rxBuffer[13];

	uint8_t buf[] = {id_device, 0x00, 0x40};
	uint8_t ile = sizeof(buf) / sizeof(uint8_t);
	uint16_t wynik = oblicz_crc(buf, ile);

	uint8_t array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x00, 0x40, array[1], array[0]};

	write(fd, tosend, sizeof(tosend));

	read(fd, rxBuffer, sizeof(rxBuffer));


	char pan_full[] = "";
	sprintf(pan_full, "0x%02X%02X", rxBuffer[4], rxBuffer[5]);
	int pan_int = strtoul(pan_full, NULL, 0);
	ptzf.pan = (float)pan_int / 50;

	char tilt_full[] = "";
	sprintf(tilt_full, "0x%02X%02X", rxBuffer[6], rxBuffer[7]);
	int tilt_int = strtoul(tilt_full, NULL, 0);
	ptzf.tilt = (float)tilt_int / 50;

	char zoom_full[] = "";
	sprintf(zoom_full, "0x%02X", rxBuffer[8]);
	ptzf.zoom = strtoul(zoom_full, NULL, 0);

	char focus_full[] = "";
	sprintf(focus_full, "0x%02X", rxBuffer[9]);
	ptzf.focus = strtoul(focus_full, NULL, 0);
	return ptzf;
}

void set_PTZF(device* dev) {
	int fd = dev->fd;
	int id = dev->id;

	if (dev->ptzf.pan > 360)
		dev->ptzf.pan = dev->ptzf.pan - 360;
	else if (dev->ptzf.pan < 0)
		dev->ptzf.pan = dev->ptzf.pan + 360;

	if (dev->ptzf.tilt > 90)
		dev->ptzf.tilt = 90;
	else if (dev->ptzf.tilt < 0)
		dev->ptzf.tilt = 0;

	if (dev->ptzf.zoom > 254)
		dev->ptzf.zoom = 254;
	else if (dev->ptzf.zoom < 0)
		dev->ptzf.zoom = 0;

	if (dev->ptzf.focus > 254)
		dev->ptzf.focus = 254;
	else if (dev->ptzf.focus < 0)
		dev->ptzf.focus = 0;

	set_pan(fd, id, dev->ptzf.pan);
	set_tilt(fd, id, dev->ptzf.tilt);
	set_zoom(fd, id, dev->ptzf.zoom);
	set_focus(fd, id, dev->ptzf.focus);
}
