#!/bin/bash

#Press ctrl-a-d to detach communications
clear

echo "Starting serial communications...."
sleep 1

#BaudRate=9600, 8 data bits, no parity bits, 1 stop bit, ignore hang-ups
sudo screen /dev/ttyACM0 9600,cs8,-parenb,-cstopb,-hupcl

