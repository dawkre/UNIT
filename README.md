# UNIT

A university project I developed with a friend. The goal of the project is to use two PTZF cameras to detect objects, triangulate their position and track them with high magnification and resolution. Video of the system in action can be found [here](https://youtu.be/GDLeBDEoyKg).

This project uses OpenCV and pylon software as dependencies. It uses custom built hardware setup.

**Hardware configuration**

After connecting the devices always run below commands to configure ports:
```
sudo setserial /dev/ttyMXUSB0 spd-vhi;
sudo setserial /dev/ttyMXUSB1 spd-vhi;
sudo setserial /dev/ttyMXUSB1 port 0x1;
sudo setserial /dev/ttyMXUSB0 port 0x1;
```

After turning on the cameras always import the camera features
from the `camera_features` directory using `PylonViewerApp`.

**Project setup**

Clone the project:

```
git clone https://github.com/dawkre/UNIT.git
cd UNIT
```

Build the project:

```
mkdir build && cd build
cmake ../src
make
```

Run as root:

`sudo ./main`
