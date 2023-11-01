#!/bin/bash

sudo echo 26 > /sys/class/gpio/export
sudo echo 19 > /sys/class/gpio/export
sudo echo 13 > /sys/class/gpio/export
sudo echo 5 > /sys/class/gpio/export
sudo echo 6 > /sys/class/gpio/export

sudo echo in > /sys/class/gpio/gpio26/direction
sudo echo in > /sys/class/gpio/gpio19/direction
sudo echo in > /sys/class/gpio/gpio13/direction
sudo echo in > /sys/class/gpio/gpio6/direction
sudo echo in > /sys/class/gpio/gpio5/direction

prevState26="1"
prevState19="1"

while true
do
	currState26=$(cat "/sys/class/gpio/gpio26/value")
	if [ "$currState26" = "$prevState26" ]
	then
		if [ "$prevState26" = '1' ]
		then
			prevState26="0"
		else
			prevState26="1"
			echo -n adelante >> /tmp/fifo
		fi
	fi
	currState19=$(cat "/sys/class/gpio/gpio19/value")
	if [ "$currState19" = "$prevState19" ]
	then
		if [ "$prevState19" = '1' ]
		then
			prevState19="0"
		else
			prevState19="1"
			echo -n enter >> /tmp/fifo
		fi
	fi
done
