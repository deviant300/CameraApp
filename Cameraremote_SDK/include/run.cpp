#include <iostream>
#include <memory>
#include "main.h"
#include <thread>

CrChar path[] = L"C:\\Users\alank\\Documents\\CameraApp\\Images";
CrChar prefix[] = L"ILXLR1_";

int main(){
    std::cout << "Sony" << std::endl;
    SCRSDK::Init();

    SCRSDK::ICrCameraObjectInfo *pCam = CreateUSBObject();
    std::cout << pCam << std::endl;

    if (pCam == nullptr)
    {
        std::cout << "Failed to create camera object." << std::endl;
        return 0;
    }
    SCRSDK::CrDeviceHandle handle = ConnectCamera(pCam);
    std::cout << handle << std::endl;

    if (handle == 0)
    { // Assuming 0 is an invalid handle
        std::cout << "Failed to connect to the camera." << std::endl;
        return 0;
    }
    std::cout << "Camera connected successfully." << std::endl;

    // Store image folder (page 70)
    SCRSDK::SetSaveInfo(handle, path, prefix, 001);

    SCRSDK::SendCommand(handle, SCRSDK::CrCommandId_Release, SCRSDK::CrCommandParam_Down);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    SCRSDK::SendCommand(handle, SCRSDK::CrCommandId_Release, SCRSDK::CrCommandParam_Up);

    // Disconnect and clean up before exiting
    SCRSDK::Disconnect(handle);

    // Clean up SDK resources
    SCRSDK::Release();
    return 0;
}