# CameraApp
## Introduction

This is a piece of code based off of Sony SDK to download pictures as they are clicked and directly store them on a raspberry pi or NVIDIA Jetson.

## How to run -new
Run run.py

## How to run -old

1. Change directories to the folder where main.cpp is stored
2. Remove the build folder
3. In CMakeLists.txt file change the file paths for libraries and directories to match their actual location in Cameraremote_SDK
4. Change directory to build folder: ```cd build```
5. Generate the build files: ```cmake ..```
6. Make the project: ```make```
7. To run use: ```./CameraApp```

#### always build the project after downloading from github

Use RemoteCli.cpp as reference

For camera change SSH password in CameraDevice.cpp line 179