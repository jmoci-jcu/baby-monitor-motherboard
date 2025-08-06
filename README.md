# Introduction

This code is intended to be run on a raspberry pi 4 and communicate with the project baby monitor daughterboard. The role of this code is to receive logs from the daughterboard and store them in a log file. Additionally, certain logs can trigger the camera to check for baby movement and record.


# Prerequisites

You must install (Installed in lab):
 - `libopencv-dev`
 - `gstreamer1.0-tools` 
 - `gstreamer1.0-plugins-base-apps`

And (Not installed in lab)
 - `libglib2.0-dev`


To add the bluez_inc library you must run:
- `git submodule update --init --recursive`


# Build and run

Make a `build` directory, and run `cmake` and then `make`.

```
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ ./BaMon
```
