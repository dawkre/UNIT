# UNIT

This project uses OpenCV and pylon software as dependencies.

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