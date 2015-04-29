## ECEN 2270 Final Software – Group 121

This repository contains all the code used for the final project of group 121.  We used a system of three microcontrollers, two of which were Arduino TeensyLCs and one of which was an Arduino Uno.

![Block Diagram](/blkdgrm.png "Block Diagram")

The code for the transmitter/sensor input TeensyLC is located in ECEN_TX.

The code for the receiver TeensyLC is located in ECEN_RX.

The code for the robot control Arduino Uno is located in Robocode.

## Requirements

Using the TeensyLC requires additional support from PJRC, downloadable [here](https://www.pjrc.com/teensy/td_download.html).

We use an Adafruit PCD8544 LCD display to monitor our transmitter, which requires a library available [here](https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library).