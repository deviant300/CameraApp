cmake_minimum_required(VERSION 3.10)
project(CameraApp LANGUAGES CXX)

# Include Camera Remote SDK headers
include_directories(C:/Users/alank/Documents/CameraApp/Cameraremote_SDK/include)

# Link Camera Remote SDK libraries
link_directories(C:/Users/alank/Documents/CameraApp/Cameraremote_SDK/lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(CameraApp main.cpp)
target_link_libraries(CameraApp PRIVATE Cr_Core Cr_PTP_IP Cr_PTP_USB usb-1.0 ssh2)