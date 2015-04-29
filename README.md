## ECEN 2270 Final Software – Group 121

This repository contains all the code used for the final project of group 121.  We used a system of four microcontrollers, two of which were Arduino TeensyLCs and one of which was an Arduino Uno.

![Block Diagram](/blkdgrm.png "Block Diagram")

## Code

The code for the transmitter/sensor input TeensyLC is located in ECEN_TX.  This microcontroller maintains configuration of the wireless module, reads sensor input, computes commands for the robot, and sends these to the wireless transmitter module.

The code for the receiver TeensyLC is located in ECEN_RX.  This microcontroller connects to the wireless receiver module, reads commands from it, and passes these commands to the Arduino.

The code for the robot control Arduino Uno is located in Robocode.  This microcontroller reads commands from the wireless receiver module and translates them into actions (in this case, driving motors).

## Requirements

Using the TeensyLC requires additional support from PJRC, downloadable [here](https://www.pjrc.com/teensy/td_download.html).

We use an Adafruit PCD8544 LCD display to monitor our transmitter, which requires a library available [here](https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library).

## About Us

We are Joe Blake, Matthew Haney, Rachel Lawson, and Milica Notaros.  Fear us.