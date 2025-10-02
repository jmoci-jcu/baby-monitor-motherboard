# Introduction

This code is part of the BaMon remote baby monitor project. It is designed to run on a Raspberry Pi 4 and communicate with the BaMon [remote unit](https://github.com/jmoci-jcu/baby-monitor). The role of this binary is to receive logs from the daughterboard and store them in a log file. The camera takes a short recording if a PIR motion, accelerometer vibration or sound alert is recieved and the camera identifies moition via object recognition.

<img src="https://github.com/user-attachments/assets/4d214f2b-e34f-4d76-b7d4-193373abecb6" width="600" />

# Prerequisites
You must install:
 - `libopencv-dev`
 - `gstreamer1.0-tools` 
 - `gstreamer1.0-plugins-base-apps`
 - `libglib2.0-dev`

Add the [BlueZ](https://github.com/weliem/bluez_inc) library:
- `git submodule update --init --recursive`


# Build and run
Make a `build` directory, and run `cmake` and then `make` on Linux.

# Hardware
Make sure you have the [latest version](https://github.com/jmoci-jcu/baby-monitor) of software loaded on the remote unit.
Linux:
```
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ ./BaMon
```
