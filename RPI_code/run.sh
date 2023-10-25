#!/bin/bash

sudo g++ Arial_16B.cpp.o main.cpp mvc.cpp -o generador -lpthread
sudo mkfifo /tmp/fifo
sudo chmod 666 /tmp/fifo
sudo ./xWindow 480 320 | sudo ./generador shared-fb
