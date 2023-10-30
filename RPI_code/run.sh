#!/bin/bash

sudo g++ Arial_16B.cpp main.cpp mvc.cpp -o gen_x86 -lpthread
sudo arm-linux-gnueabihf-g++ Arial_16B.cpp main.cpp mvc.cpp -o gen_arm -lpthread --static
sudo mkfifo /tmp/fifo
sudo chmod 666 /tmp/fifo
sudo ./xWindow 480 320 | sudo ./gen_x86 shared-fb
