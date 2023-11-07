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
prevState13="1"
prevState6="1"
prevState5="1"

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
			echo -n atras >> /tmp/fifo
		fi
	fi
	
	currState13=$(cat "/sys/class/gpio/gpio13/value")
	if [ "$currState13" = "$prevState13" ]
	then
		if [ "$prevState13" = '1' ]
		then
			prevState13="0"
		else
			prevState13="1"
			echo -n enter >> /tmp/fifo
		fi
	fi
	
	currState6=$(cat "/sys/class/gpio/gpio6/value")
	if [ "$currState6" = "$prevState6" ]
	then
		if [ "$prevState6" = '1' ]
		then
			prevState6="0"
		else
			prevState6="1"
			echo -n inc >> /tmp/fifo
		fi
	fi
	
	currState5=$(cat "/sys/class/gpio/gpio5/value")
	if [ "$currState5" = "$prevState5" ]
	then
		if [ "$prevState5" = '1' ]
		then
			prevState5="0"
		else
			prevState5="1"
			echo -n dec >> /tmp/fifo
		fi
	fi
	
done
