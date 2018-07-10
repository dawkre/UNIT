#include "PTZF_control.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
		printf("%s: Unable to open.\n", deviceName);
		return -1;
	}
	return fd;
}

int oblicz_crc(int* bufor, int ilosc)
{
	int crc = 0xffff;
	int i, j;
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
	printf("%c\n", buf);
	return buf;
}

void set_control(int fd, int id_device)
{
	int buf[] = {id_device, 0x04, 0x51, 0x80, 0x80, 0x80, 0x10};
	int ile = sizeof(buf) / sizeof(int);
	int wynik = oblicz_crc(buf, ile);

	int array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x04, 0x51, 0x80, 0x80, 0x80, 0x10, array[1], array[0]};
	/* Send content to RS485 */
	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

void set_PTZF(int fd, int id_device, float pan, float tilt, int speed_pan, int speed_tilt, int zoom, int focus)
{
	int position_pan = (int)(50 * pan);
	int position_tilt = (int)(50 * tilt);
	char hex_pan[5], hex_tilt[5], hex_speed_pan[5], hex_speed_tilt[5], hex_zoom[5], hex_focus[5];
	char hex_low_pan[4], hex_high_pan[4], hex_low_tilt[4], hex_high_tilt[4];

	sprintf(hex_pan, "%04X", position_pan);
	hex_high_pan[0] = '0';
	hex_high_pan[1] = 'x';
	hex_high_pan[2] = hex_pan[0];
	hex_high_pan[3] = hex_pan[1];
	hex_low_pan[0] = '0';
	hex_low_pan[1] = 'x';
	hex_low_pan[2] = hex_pan[2];
	hex_low_pan[3] = hex_pan[3];

	sprintf(hex_tilt, "%04X", position_tilt);
	hex_high_tilt[0] = '0';
	hex_high_tilt[1] = 'x';
	hex_high_tilt[2] = hex_tilt[0];
	hex_high_tilt[3] = hex_tilt[1];
	hex_low_tilt[0] = '0';
	hex_low_tilt[1] = 'x';
	hex_low_tilt[2] = hex_tilt[2];
	hex_low_tilt[3] = hex_tilt[3];

	sprintf(hex_speed_pan, "0x%02X", speed_pan);
	sprintf(hex_speed_tilt, "0x%02X", speed_tilt);
	sprintf(hex_zoom, "0x%02X", zoom);
	sprintf(hex_focus, "0x%02X", focus);

	int uint_low_pan = strtoul(hex_low_pan, NULL, 0);
	int uint_high_pan = strtoul(hex_high_pan, NULL, 0);
	int uint_low_tilt = strtoul(hex_low_tilt, NULL, 0);
	int uint_high_tilt = strtoul(hex_high_tilt, NULL, 0);
	int uint_speed_pan = strtoul(hex_speed_pan, NULL, 0);
	int uint_speed_tilt = strtoul(hex_speed_tilt, NULL, 0);
	int uint_zoom = strtoul(hex_zoom, NULL, 0);
	int uint_focus = strtoul(hex_focus, NULL, 0);

	int buf[] = {id_device, 0x09, 0x30, uint_high_pan, uint_low_pan, uint_high_tilt, uint_low_tilt, uint_speed_pan, uint_speed_tilt, uint_zoom, uint_focus, 0x05};
	int ile = sizeof(buf) / sizeof(int);
	int wynik = oblicz_crc(buf, ile);

	int array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x09, 0x30, uint_high_pan, uint_low_pan, uint_high_tilt, uint_low_tilt, uint_speed_pan, uint_speed_tilt, uint_zoom, uint_focus, 0x05, array[1], array[0]};

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

	int uint_low = strtoul(hex_low, NULL, 0);
	int uint_high = strtoul(hex_high, NULL, 0);

	int buf[] = {id_device, 0x02, 0x31, uint_high, uint_low};
	int ile = sizeof(buf) / sizeof(int);
	int wynik = oblicz_crc(buf, ile);

	int array[2];
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

	int uint_low = strtoul(hex_low, NULL, 0);
	int uint_high = strtoul(hex_high, NULL, 0);

	int buf[] = {id_device, 0x02, 0x32, uint_high, uint_low};
	int ile = sizeof(buf) / sizeof(int);
	int wynik = oblicz_crc(buf, ile);

	int array[2];
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

	int uint_zoom = strtoul(hex_full, NULL, 0);
	int buf[] = {id_device, 0x01, 0x33, uint_zoom};
	int ile = sizeof(buf) / sizeof(int);
	int wynik = oblicz_crc(buf, ile);

	int array[2];
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

	int uint_focus = strtoul(hex_full, NULL, 0);
	int buf[] = {id_device, 0x01, 0x34, uint_focus};
	int ile = sizeof(buf) / sizeof(int);
	int wynik = oblicz_crc(buf, ile);

	int array[2];
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

	int uint_speed = strtoul(hex_full, NULL, 0);
	int buf[] = {id_device, 0x04, 0x51, uint_speed, 0x80, 0x80, 0x10};
	int ile = sizeof(buf) / sizeof(int);
	int wynik = oblicz_crc(buf, ile);

	int array[2];
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

	int uint_speed = strtoul(hex_full, NULL, 0);
	int buf[] = {id_device, 0x04, 0x51, 0x80, uint_speed, 0x80, 0x10};
	int ile = sizeof(buf) / sizeof(int);
	int wynik = oblicz_crc(buf, ile);

	int array[2];
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

	int uint_speed = strtoul(hex_full, NULL, 0);
	int buf[] = {id_device, 0x04, 0x51, 0x80, 0x80, uint_speed, 0x10};
	int ile = sizeof(buf) / sizeof(int);
	int wynik = oblicz_crc(buf, ile);

	int array[2];
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

	int uint_speed = strtoul(hex_full, NULL, 0);
	int buf[] = {id_device, 0x04, 0x51, 0x80, 0x80, 0x80, uint_speed};
	int ile = sizeof(buf) / sizeof(int);
	int wynik = oblicz_crc(buf, ile);

	int array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x04, 0x51, 0x80, 0x80, 0x80, uint_speed, array[1], array[0]};
	/* Send content to RS485 */
	if (write(fd, tosend, sizeof(tosend)) != sizeof(tosend))
		printf("Write() failed\n");
}

void get_position(int fd, int id_device, float pan, float tilt, int zoom, int focus)
{
	int rxBuffer[13];

	int buf[] = {id_device, 0x00, 0x40};
	int ile = sizeof(buf) / sizeof(int);
	int wynik = oblicz_crc(buf, ile);

	int array[2];
	array[0] = wynik & 0xff;
	array[1] = (wynik >> 8);

	char tosend[] = {0xA5, id_device, 0x00, 0x40, array[1], array[0]};

	write(fd, tosend, sizeof(tosend));

	read(fd, rxBuffer, sizeof(rxBuffer));

	int i;
	for (i = 0; i < sizeof(rxBuffer); i++)
		printf("HEX: %02X\n", rxBuffer[i]);

	char pan_full[] = "";
	sprintf(pan_full, "0x%02X%02X", rxBuffer[4], rxBuffer[5]);
	int pan_int = strtoul(pan_full, NULL, 0);
	pan = (float)pan_int / 50;
	printf("Pan_int: %d\n", pan_int);
	printf("Pan: %f\n", pan);

	char tilt_full[] = "";
	sprintf(tilt_full, "0x%02X%02X", rxBuffer[6], rxBuffer[7]);
	int tilt_int = strtoul(tilt_full, NULL, 0);
	tilt = (float)tilt_int / 50;
	printf("Tilt_int: %d\n", tilt_int);
	printf("Tilt: %f\n", tilt);

	char zoom_full[] = "";
	sprintf(zoom_full, "0x%02X", rxBuffer[8]);
	zoom = strtoul(zoom_full, NULL, 0);
	printf("Zoom: %d\n", zoom);

	char focus_full[] = "";
	sprintf(focus_full, "0x%02X", rxBuffer[9]);
	focus = strtoul(focus_full, NULL, 0);
	printf("Focus: %d\n\n", focus);
}
